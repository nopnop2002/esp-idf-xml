# esp-idf-xml
Example of XML parser with ESP-IDF.

ESP-IDF includes [this](https://libexpat.github.io/) XML parser library (Version: 2.2.5).   
You can use XML parser as standard.
But there is no example code.   

# How to use
git clone https://github.com/nopnop2002/esp-idf-xml
cd esp-idf-xml
make flash monitor

# How to change XML source
- Edit getpem.sh and set the URL.
```
openssl s_client -showcerts -connect weather-broker-cdn.api.bbci.co.uk:443 </dev/null >hoge
#openssl s_client -showcerts -connect www.espn.com:443 </dev/null >hoge
```

- Execute getpem.sh

- Edit main/http_client.c and set the URL.
```
    strcpy(url, "https://weather-broker-cdn.api.bbci.co.uk/en/forecast/rss/3day/2643743");
    //strcpy(url, "https://www.espn.com/espn/rss/news");
```

- Build firmware
