#!/bin/bash
ROOT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cd $ROOT_DIR
rm -fr app_build
mkdir app_build
cd app_build
cmake .. \
    -DCMAKE_PREFIX_PATH=$ROOT_DIR/tflite_install \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON 
cmake --build . -j4
