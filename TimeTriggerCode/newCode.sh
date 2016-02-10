#!/bin/bash

cat /media/sf_Thesis/MyTimeTrigger.cpp > MyTimeTrigger.cpp
cat /media/sf_Thesis/MyTimeTrigger.hpp > MyTimeTrigger.hpp
cat /media/sf_Thesis/configuration > configuration

g++ -I /usr/include/opendavinci -c MyTimeTrigger.cpp -o MyTimeTrigger.o
g++ -o timetrigger MyTimeTrigger.o -lopendavinci -lpthread -lrt

