#!/bin/bash
ROOT_DIR=$(pwd)
rm -fr $ROOT_DIR/build
rm -f ~/.conan2/profiles/ubuntu-gcc11-release
cp $ROOT_DIR/conan/ubuntu-gcc11-release ~/.conan2/profiles/
conan profile show -pr ubuntu-gcc11-release
conan install conanfile.py --build=missing --profile=ubuntu-gcc11-release
cmake --preset conan-release
cmake --build build --preset conan-release