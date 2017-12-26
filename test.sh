#!/bin/bash

g++ -Wall -g -std=c++14 -pthread -I. test.m.cpp Logger.cpp LogReader.cpp SafePrint.cpp -o test && ./test

