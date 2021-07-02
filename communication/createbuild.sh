#!/bin/bash
rm -rf build
mkdir build
cd build
cmake ../can_driver
make
