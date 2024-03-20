#!/bin/bash

mkdir -p build && cd build && cmake ../ && make
cd .. && ./create_links.sh
