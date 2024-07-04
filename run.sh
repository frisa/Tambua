#!/bin/bash
ROOT_DIR=$(pwd)
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
CLONE=0
CLEAN=1

cd $ROOT_DIR
if [ $CLONE -eq 1 ]; then
    rm -fr tensorflow_src
    git clone https://github.com/tensorflow/tensorflow.git tensorflow_src
fi

if [ $CLEAN -eq 1 ]; then
    rm -fr tensorflow_build
    rm -fr tensorflow_install
    mkdir tensorflow_build
fi

cd tensorflow_build
cmake ../tensorflow_src/tensorflow/lite \
    -DTFLLITE_ENABLE_INSTALL=ON \
    -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON \
    -DSYSTEM_FARMHASH=OFF \
    -DSYSTEM_PTHREADPOOL=ON \
    -DABSL_PROPAGATE_CXX_STD=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$ROOT_DIR/tensorflow_install \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DTFLITE_ENABLE_RUY=OFF \
    -DTFLITE_ENABLE_NNAPI=OFF \
    -DTFLITE_ENABLE_GPU=OFF \
    -DTFLITE_ENABLE_XNNPACK=OFF \
    -DTFLITE_ENABLE_MMAP=OFF \
    -Wno-dev

make -j $(nproc)
make install
exit 0
cd $ROOT_DIR
rm -fr build
mkdir build
cd build
cmake .. 
make -j4
