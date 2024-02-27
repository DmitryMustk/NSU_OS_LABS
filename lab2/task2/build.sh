#!/bin/bash

mkdir -p build && cd build
as -o main.o ../src/main.s
ld -o main main.o
rm main.o