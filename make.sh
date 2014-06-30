#!/bin/sh

g++ -o s1 s1.cc s1_stat_handler.cc redis.cc -I . -I ~/lib/include/ -L /home/AustinChen/lib/ -lfcgi -g -O2 -Wall -lhiredis
g++ -o p1 p1.cc -I . -I ~/lib/include/ -I /usr/local/include/MPFDParser-1 -lMPFDParser-1 -L /home/AustinChen/lib/ -lfcgi -g -O2 -Wall
