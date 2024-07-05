#!/bin/bash
ROOT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
CLONE=0
CLEAN=1

cd $ROOT_DIR
if [ $CLONE -eq 1 ]; then
    rm -fr tflite_src
    git clone https://github.com/tensorflow/tensorflow.git tflite_src
fi

if [ $CLEAN -eq 1 ]; then
    rm -fr tflite_build
    rm -fr tflite_install
    mkdir tflite_build
fi

cd tflite_build
cmake ../tflite_src/tensorflow/lite \
    -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON \
    -DSYSTEM_FARMHASH=OFF \
    -DSYSTEM_PTHREADPOOL=OFF \
    -DABSL_PROPAGATE_CXX_STD=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$ROOT_DIR/tflite_install \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DTFLITE_ENABLE_INSTALL=ON \
    -DTFLITE_ENABLE_LABEL_IMAGE=OFF \
    -DTFLITE_ENABLE_RUY=OFF \
    -DTFLITE_ENABLE_RESOURCE=OFF \
    -DTFLITE_ENABLE_NNAPI=OFF \
    -DTFLITE_ENABLE_GPU=OFF \
    -DTFLITE_ENABLE_METAL=OFF \
    -DTFLITE_ENABLE_XNNPACK=OFF \
    -DTFLITE_ENABLE_EXTERNAL_DELEGATES=OFF \
    -DTFLITE_KERNEL_TEST=OFF \
    -DTFLITE_ENABLE_MMAP=OFF \
    -Wno-dev

cmake --build . -j4
cmake --install .
