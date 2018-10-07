#!/bin/sh
rm /home/debian/testing-suite/log.txt;

cd /home/debian/testing-suite/;
./dataRead > log.txt
