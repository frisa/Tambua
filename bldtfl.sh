#!/bin/bash
ROOT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
CLONE=0
CLEAN=1

cd $ROOT_DIR
if [ $CLONE -eq 1 ]; then
    echo "Cloning tflite source..."
    rm -fr tflite_src
    git clone https://github.com/tensorflow/tensorflow.git tflite_src
fi

if [ $CLEAN -eq 1 ]; then
    echo "Cleaning up..."
    rm -fr build
    rm -fr tflite_build
    rm -fr tflite_install
    mkdir tflite_build
fi
echo "Building tflite..."
cd tflite_build
cmake ../tflite_src/tensorflow/lite \
    -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON \
    -DTFLITE_ENABLE_INSTALL=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$ROOT_DIR/tflite_install \
    -DABSL_ENABLE_INSTALL=ON \
    -DTFLITE_ENABLE_XNNPACK=OFF \
    -Wno-dev
cmake --build . -j4
cmake --install .
