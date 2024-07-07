#!/bin/bash
ROOT_DIR=$(pwd)

cp $ROOT_DIR/conan/ubuntu-gcc11-release ~/.conan/profiles/
conan profile show ubuntu-gcc11-release
conan install conanfile.py --build=missing --profile ubuntu-gcc11-release