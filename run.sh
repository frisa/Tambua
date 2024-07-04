#!/bin/bash
ROOT_DIR=$(pwd)
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cd $ROOT_DIR
rm -fr tensorflow_src
git clone https://github.com/tensorflow/tensorflow.git tensorflow_src

rm -fr tensorflow_build
mkdir tensorflow_build
cd tensorflow_build
cmake ../tensorflow_src/tensorflow/lite \
    -DTFLLITE_ENABLE_INSTALL=ON \
    -DABSL_PROPAGATE_CXX_STD=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON \
    -DCMAKE_INSTALL_PREFIX=$ROOT_DIR/tensorflow_install \
    -Wno-dev

cmake --build . --target install