#!/bin/bash

g++ -Wall -std=c++14 -pthread -I. test.m.cpp Logger.cpp LogReader.cpp QueueManager.cpp LogStruct.cpp

