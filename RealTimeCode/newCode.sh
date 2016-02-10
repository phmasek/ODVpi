#!/bin/bash

cat /media/sf_Thesis/MyRealtimeService.cpp > MyRealtimeService.cpp
cat /media/sf_Thesis/MyRealtimeService.hpp > MyRealtimeService.hpp

g++ -I /usr/include/opendavinci -c MyRealtimeService.cpp -o MyRealtimeService.o
g++ -o realtimeservice MyRealtimeService.o -lopendavinci -lpthread -lrt
