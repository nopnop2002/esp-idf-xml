# esp-idf-xml
IDF component registry includes Expat XML Parsing C Library.   
https://components.espressif.com/components/espressif/expat   

However, unfortunately there is no sample code for ESP-IDF.   
This is example XML parser using Expat.   

# Software requirements
esp-idf v4.4/v5.x.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-xml
cd esp-idf-xml
chmod 777 ./getpem.sh
./getpem.sh
idf.py menuconfig
idf.py flash monitor
```

# Configuration
Set the following items using menuconfig.
- CONFIG_ESP_WIFI_SSID   
SSID of your wifi.
- CONFIG_ESP_WIFI_PASSWORD   
PASSWORD of your wifi.
- CONFIG_ESP_MAXIMUM_RETRY   
Maximum number of retries when connecting to wifi.

![config-main](https://user-images.githubusercontent.com/6020549/100534161-2fc24c80-324f-11eb-9aa1-5db665283741.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/100534164-32bd3d00-324f-11eb-98f0-9218125ad7c4.jpg)

# XML source
XML source for this projec is BBC Weather Forecast for London.   
https://weather-broker-cdn.api.bbci.co.uk/en/forecast/rss/3day/2643743


# Console output
```
I (8029) HTTP: tag=[rss/channel/title]
I (8029) HTTP: element=[BBC Weather - Forecast for  London, GB]
I (8039) HTTP: tag=[rss/channel/link]
I (8039) HTTP: element=[https://www.bbc.co.uk/weather/2643743]
I (8049) HTTP: tag=[rss/channel/description]
I (8049) HTTP: element=[3-day forecast for London from BBC Weather, including weather, temperature and wind information]
I (8069) HTTP: tag=[rss/channel/language]
I (8069) HTTP: element=[en]
I (8069) HTTP: tag=[rss/channel/copyright]
I (8079) HTTP: element=[Copyright: (C) British Broadcasting Corporation, see https://www.bbc.co.uk/usingthebbc/terms-of-use/usingthebbc/terms-of-use/#d7c3c6fd18ea551b9f3f26ecbd4612be for more details]
I (8099) HTTP: tag=[rss/channel/pubDate]
I (8099) HTTP: element=[Wed, 11 May 2022 05:22:20 GMT]
I (8109) HTTP: tag=[rss/channel/dc:date]
I (8109) HTTP: element=[2022-05-11T05:22:20Z]
I (8119) HTTP: tag=[rss/channel/dc:language]
I (8119) HTTP: element=[en]
```

# How to change XML source
- Edit getpem.sh and set the URL.
```
openssl s_client -showcerts -connect weather-broker-cdn.api.bbci.co.uk:443 </dev/null >hoge
#openssl s_client -showcerts -connect www.espn.com:443 </dev/null >hoge
```

- Run getpem.sh to get the public key.
```
$ ./getpem.sh
depth=2 OU = GlobalSign Root CA - R3, O = GlobalSign, CN = GlobalSign
verify return:1
depth=1 C = BE, O = GlobalSign nv-sa, CN = GlobalSign RSA OV SSL CA 2018
verify return:1
depth=0 C = GB, ST = London, L = London, O = British Broadcasting Corporation, CN = www.bbc.co.uk
verify return:1
DONE
-----BEGIN CERTIFICATE-----
MIIETjCCAzagAwIBAgINAe5fFp3/lzUrZGXWajANBgkqhkiG9w0BAQsFADBXMQsw
CQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UECxMH
Um9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTE4MDkxOTAw
MDAwMFoXDTI4MDEyODEyMDAwMFowTDEgMB4GA1UECxMXR2xvYmFsU2lnbiBSb290
IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNpZ24xEzARBgNVBAMTCkdsb2JhbFNp
Z24wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDMJXaQeQZ4Ihb1wIO2
hMoonv0FdhHFrYhy/EYCQ8eyip0EXyTLLkvhYIJG4VKrDIFHcGzdZNHr9SyjD4I9
DCuul9e2FIYQebs7E4B3jAjhSdJqYi8fXvqWaN+JJ5U4nwbXPsnLJlkNc96wyOkm
DoMVxu9bi9IEYMpJpij2aTv2y8gokeWdimFXN6x0FNx04Druci8unPvQu7/1PQDh
BjPogiuuU6Y6FnOM3UEOIDrAtKeh6bJPkC4yYOlXy7kEkmho5TgmYHWyn3f/kRTv
riBJ/K1AFUjRAjFhGV64l++td7dkmnq/X8ET75ti+w1s4FRpFqkD2m7pg5NxdsZp
hYIXAgMBAAGjggEiMIIBHjAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB
/zAdBgNVHQ4EFgQUj/BLf6guRSSuTVD6Y5qL3uLdG7wwHwYDVR0jBBgwFoAUYHtm
GkUNl8qJUC99BM00qP/8/UswPQYIKwYBBQUHAQEEMTAvMC0GCCsGAQUFBzABhiFo
dHRwOi8vb2NzcC5nbG9iYWxzaWduLmNvbS9yb290cjEwMwYDVR0fBCwwKjAooCag
JIYiaHR0cDovL2NybC5nbG9iYWxzaWduLmNvbS9yb290LmNybDBHBgNVHSAEQDA+
MDwGBFUdIAAwNDAyBggrBgEFBQcCARYmaHR0cHM6Ly93d3cuZ2xvYmFsc2lnbi5j
b20vcmVwb3NpdG9yeS8wDQYJKoZIhvcNAQELBQADggEBACNw6c/ivvVZrpRCb8RD
M6rNPzq5ZBfyYgZLSPFAiAYXof6r0V88xjPy847dHx0+zBpgmYILrMf8fpqHKqV9
D6ZX7qw7aoXW3r1AY/itpsiIsBL89kHfDwmXHjjqU5++BfQ+6tOfUBJ2vgmLwgtI
fR4uUfaNU9OrH0Abio7tfftPeVZwXwzTjhuzp3ANNyuXlava4BJrHEDOxcd+7cJi
WOx37XMiwor1hkOIreoTbv3Y/kIvuX1erRjvlJDKPSerJpSZdcfL03v3ykzTr1Eh
kluEfSufFT90y1HonoMOFm8b50bOI7355KKL0jlrqnkckSziYSQtjipIcJDEHsXo
4HA=
-----END CERTIFICATE-----
```

- Edit main/http_client.c and set the URL.
```
strcpy(url, "https://weather-broker-cdn.api.bbci.co.uk/en/forecast/rss/3day/2643743");
//strcpy(url, "https://www.espn.com/espn/rss/news");
```

- Build firmware


