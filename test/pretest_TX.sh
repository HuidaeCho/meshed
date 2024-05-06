#!/bin/sh
(cd ..; make)
curl https://data.isnew.info/meshed/inputs_TX.zip -o inputs_TX.zip
unzip inputs_TX.zip
mkdir outputs_TX
