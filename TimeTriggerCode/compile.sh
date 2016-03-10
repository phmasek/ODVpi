g++ -std=c++11 -I /usr/local/include -c TimeTrigger.cpp -o TimeTrigger.o
g++ -o timetrigger TimeTrigger.o -lopendavinci -lpthread
