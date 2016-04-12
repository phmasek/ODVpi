#!/bin/bash

g++ -g -std=c++11 -I /opt/od3/include -c src/TimeTrigger.cpp -o TimeTrigger.o
g++ -g -std=c++11 -I /opt/od3/include -c src/OpenCVCamera.cpp -o OpenCVCamera.o
g++ -g -std=c++11 -I /opt/od3/include -c src/Camera.cpp -o Camera.o
g++ -o timetriggerIO Camera.o OpenCVCamera.o TimeTrigger.o -lopendavinci -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect
