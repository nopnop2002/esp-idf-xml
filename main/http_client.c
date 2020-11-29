/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <expat.h>
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_http_client.h" 
#include "esp_tls.h" 
#include "cJSON.h"



static const char *TAG = "HTTP";

//EventGroupHandle_t xEventGroup;
extern EventGroupHandle_t xEventGroup;
/* Is the Enter key entered */
//const int KEYBOARD_ENTER_BIT = BIT2;
extern int KEYBOARD_ENTER_BIT;

extern const char weather_yahoo_cert_pem_start[] asm("_binary_cert_pem_start");
extern const char weather_yahoo_cert_pem_end[]	asm("_binary_cert_pem_end");

typedef struct {
	int		depth;						// XML depth
	char	tag[64];					// XML tag
} user_data_t;


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
	static char *output_buffer;  // Buffer to store response of http request from event handler
	static int output_len;		 // Stores number of bytes read
	switch(evt->event_id) {
		case HTTP_EVENT_ERROR:
			ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
			break;
		case HTTP_EVENT_ON_CONNECTED:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
			break;
		case HTTP_EVENT_HEADER_SENT:
			ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
			break;
		case HTTP_EVENT_ON_HEADER:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
			break;
		case HTTP_EVENT_ON_DATA:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
			ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, output_len=%d", output_len);
			ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, content_length=%d", esp_http_client_get_content_length(evt->client));
			// If user_data buffer is configured, copy the response into the buffer
			if (evt->user_data) {
				memcpy(evt->user_data + output_len, evt->data, evt->data_len);
			} else {
				if (output_buffer == NULL && esp_http_client_get_content_length(evt->client) > 0) {
					output_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
					output_len = 0;
					if (output_buffer == NULL) {
						ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
						return ESP_FAIL;
					}
				}
				memcpy(output_buffer + output_len, evt->data, evt->data_len);
			}
			output_len += evt->data_len;
			break;
		case HTTP_EVENT_ON_FINISH:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
			if (output_buffer != NULL) {
				// Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
				// ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
				free(output_buffer);
				output_buffer = NULL;
			}
			output_len = 0;
			break;
		case HTTP_EVENT_DISCONNECTED:
			ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
			int mbedtls_err = 0;
			esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
			if (err != 0) {
				if (output_buffer != NULL) {
					free(output_buffer);
					output_buffer = NULL;
				}
				output_len = 0;
				ESP_LOGE(TAG, "Last esp error code: 0x%x", err);
				ESP_LOGE(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
			}
			break;
	}
	return ESP_OK;
}


size_t http_client_content_length(char * url)
{
	ESP_LOGI(TAG, "http_client_content_length url=%s",url);
	size_t content_length;
	
	esp_http_client_config_t config = {
		.url = url,
		.event_handler = _http_event_handler,
		//.user_data = local_response_buffer,		   // Pass address of local buffer to get response
		.cert_pem = weather_yahoo_cert_pem_start,
	};
	esp_http_client_handle_t client = esp_http_client_init(&config);

	// GET
	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		ESP_LOGD(TAG, "HTTP GET Status = %d, content_length = %d",
				esp_http_client_get_status_code(client),
				esp_http_client_get_content_length(client));
		content_length = esp_http_client_get_content_length(client);

	} else {
		ESP_LOGW(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
		content_length = 0;
	}
	esp_http_client_cleanup(client);
	return content_length;
}

esp_err_t http_client_content_get(char * url, char * response_buffer)
{
	ESP_LOGI(TAG, "http_client_content_get url=%s",url);

	esp_http_client_config_t config = {
		.url = url,
		.event_handler = _http_event_handler,
		.user_data = response_buffer,		   // Pass address of local buffer to get response
		.cert_pem = weather_yahoo_cert_pem_start,
	};
	esp_http_client_handle_t client = esp_http_client_init(&config);

	// GET
	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
				esp_http_client_get_status_code(client),
				esp_http_client_get_content_length(client));
		ESP_LOGD(TAG, "\n%s", response_buffer);
	} else {
		ESP_LOGW(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
	}
	esp_http_client_cleanup(client);
	return err;
}

static void XMLCALL start_element(void *userData, const XML_Char *name, const XML_Char **atts)
{
	ESP_LOGD(TAG, "start_element name=%s", name);
	user_data_t *user_data = (user_data_t *) userData;
	int depth = user_data->depth;
	if (depth == 0) {
		strcpy(user_data->tag, name);
	} else {
		strcat(user_data->tag, "/");
		strcat(user_data->tag, name);
	}
	++user_data->depth;
}

static void XMLCALL end_element(void *userData, const XML_Char *name)
{
	ESP_LOGD(TAG, "end_element name[%d]=%s", strlen(name), name);
	user_data_t *user_data = (user_data_t *) userData;
	int tagLen = strlen(user_data->tag);
	int offset = tagLen - strlen(name) -1;
	user_data->tag[offset] = 0;
	ESP_LOGD(TAG, "tag=[%s]", user_data->tag);
	//int depth = user_data->depth;
	--user_data->depth;
}

static void data_handler(void *userData, const XML_Char *s, int len)
{
	user_data_t *user_data = (user_data_t *) userData;
	//int depth = user_data->depth;
	if (len == 1 && s[0] == 0x0a) return;
	
	int nonSpace = 0;
	for (int i=0;i<len; i++) {
		if (s[i] != 0x20) nonSpace++;
	}
	if (nonSpace == 0) return;

	ESP_LOGI(TAG, "tag=[%s]", user_data->tag);
	ESP_LOGD(TAG, "depth=%d len=%d s=[%.*s]", user_data->depth, len, len, s);
	ESP_LOGI(TAG, "element=[%.*s]", len, s);
	ESP_LOG_BUFFER_HEXDUMP(TAG, s, len, ESP_LOG_DEBUG);
}

void http_client(void *pvParameters)
{
	ESP_LOGI(TAG, "Start");
	char url[128];
	strcpy(url, "https://weather-broker-cdn.api.bbci.co.uk/en/forecast/rss/3day/2643743");
	//strcpy(url, "https://www.espn.com/espn/rss/news");
	
	// Get content length
	size_t content_length;
	while (1) {
		content_length = http_client_content_length(url);
		ESP_LOGI(TAG, "content_length=%d", content_length);
		if (content_length > 0) break;
		vTaskDelay(100);
	}

	char *response_buffer;	// Buffer to store response of http request from event handler
	response_buffer = (char *) malloc(content_length+1);
	if (response_buffer == NULL) {
		ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
		while(1) {
			vTaskDelay(1);
		}
	}
	bzero(response_buffer, content_length+1);

	// Get content
	while(1) {
		esp_err_t err = http_client_content_get(url, response_buffer);
		if (err == ESP_OK) break;
		vTaskDelay(100);
	}
	ESP_LOGI(TAG, "content_length=%d", content_length);
	ESP_LOGI(TAG, "\n[%s]", response_buffer);

	user_data_t userData;
	userData.depth = 0;
	memset(userData.tag, 0, sizeof(userData.tag));

	// Parse XML
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, &userData);
	XML_SetElementHandler(parser, start_element, end_element);
	XML_SetCharacterDataHandler(parser, data_handler);
	if (XML_Parse(parser, response_buffer, content_length, 1) != XML_STATUS_OK) {
		ESP_LOGE(TAG, "XML_Parse fail");
		while(1) {
			vTaskDelay(1);
		}
	}
	XML_ParserFree(parser);

	free(response_buffer);
	ESP_LOGI(TAG, "ExpatVersion:%s", XML_ExpatVersion());

	while(1) {
		vTaskDelay(1);
	}
}
