#!/bin/bash
mkdir out;
g++ -pthread \
  lib/**/*.cpp \
  src/*.cpp src/**/*.cpp \
  src/**/**/*.cpp \
  src/**/**/**/*.cpp \
  src/**/**/**/**/*.cpp \
  -o out/lab && ./$_
