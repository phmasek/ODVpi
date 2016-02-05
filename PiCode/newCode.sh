#!/bin/bash

cat /media/sf_Thesis/TimeTriggerExample.cpp > TimeTriggerExample.cpp
g++ -I /usr/include/opendavinci -c TimeTriggerExample.cpp -o TimeTriggerExample.o
g++ -o timetriggerexample TimeTriggerExample.o -lopendavinci -lpthread -lrt
