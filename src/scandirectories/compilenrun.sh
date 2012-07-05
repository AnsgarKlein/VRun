#!/bin/bash

clear && clear && clear

rm scandirectories
CFLAGS="-Wall -g " make scandirectories

./scandirectories
#valgrind ./scandirectories
