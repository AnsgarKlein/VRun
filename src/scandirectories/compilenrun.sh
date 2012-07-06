#!/bin/bash

clear && clear && clear

rm scandirectories && clear
CFLAGS="-Wall -g -D WITHMAIN" make scandirectories

./scandirectories
#valgrind ./scandirectories
