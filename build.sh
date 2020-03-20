#!/bin/bash

if [ ! -d "${PWD}/build" ]; then
mkdir build
else
echo "${PWD}/build exist"
fi

cd $PWD/build
cmake ..
make

