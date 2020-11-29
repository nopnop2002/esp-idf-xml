#!/bin/bash

#set -x

openssl s_client -showcerts -connect weather-broker-cdn.api.bbci.co.uk:443 </dev/null >hoge
#openssl s_client -showcerts -connect www.espn.com:443 </dev/null >hoge

start=`grep -e "-----BEGIN CERTIFICATE-----" -n hoge | sed -e 's/:.*//g' | tail -n 1`

last=`grep -e "-----END CERTIFICATE-----" -n hoge | sed -e 's/:.*//g' | tail -n 1`

sed -n ${start},${last}p hoge > main/cert.pem

cat main/cert.pem

rm hoge
