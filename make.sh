#!/bin/sh

g++ -o s1 *.cc -I . -I ~/lib/include/ -L /home/AustinChen/lib/ -lfcgi
