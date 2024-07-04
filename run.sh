#!/bin/bash
ROOT_DIR=$(pwd)

# Build the project
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

# Get tensor flow
cd $ROOT_DIR
#rm -fr tensorflow_src
#git clone https://github.com/tensorflow/tensorflow.git tensorflow_src
rm -fr tensorflow_build
mkdir tensorflow_build
cd tensorflow_build
cmake ../tensorflow_src/tensorflow/lite \
    -DABSL_PROPAGATE_CXX_STD=ON \
    -Wno-dev