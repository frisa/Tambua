#!/bin/bash
ROOT_DIR=$(pwd)
HOME_DIR=$(echo ~)

source $ROOT_DIR/conan2/bin/activate
conan --version

rm -fr $ROOT_DIR/build
rm -f $HOME_DIR/.conan2/profiles/ubuntu-gcc11-release
rm -f $HOME_DIR/.conan2/profiles/ubuntu-gcc11-debug
#rm -fr ~/.conan2/p
cp $ROOT_DIR/conan/ubuntu-gcc11-release $HOME_DIR/.conan2/profiles/
cp $ROOT_DIR/conan/ubuntu-gcc11-debug $HOME_DIR/.conan2/profiles/

conan profile show -pr ubuntu-gcc11-release
conan profile show -pr ubuntu-gcc11-debug
conan install conanfile.py --build=missing --profile=ubuntu-gcc11-debug

cmake --preset conan-debug
cmake --build build --preset conan-debug