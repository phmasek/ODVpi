#!/bin/bash

cat /media/sf_Thesis/rt_runner.c > rt_runner.c
gcc -o rt_runner rt_runner.c -lrt
