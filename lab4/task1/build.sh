#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <*> to choose versoin"
    exit 1
fi

VERSION=$1

mkdir -p build && cd build && cmake -DVERSION=$VERSION .. && make
