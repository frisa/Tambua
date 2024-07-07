#!/bin/bash
ROOT_DIR=$(pwd)

cp $ROOT_DIR/conan/ubuntu-gcc11-release ~/.conan/profiles/
conan profile show ubuntu-gcc11-release
conan install conanfile.py --output-folder=build --build=missing -pr:b=ubuntu-gcc11-release -pr:h=ubuntu-gcc11-release
cmake --preset release
cmake --build build --preset release