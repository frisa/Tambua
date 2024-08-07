#!/bin/bash
ROOT_DIR=$(pwd)
HOME_DIR=$(echo ~)

#source $ROOT_DIR/venv/bin/activate
conan --version

rm -fr $ROOT_DIR/build
#rm -fr ~/.conan2/p

rm -f $HOME_DIR/.conan2/profiles/ubuntu-gcc11-release
rm -f $HOME_DIR/.conan2/profiles/ubuntu-gcc11-debug

cp $ROOT_DIR/conan/ubuntu-gcc11-release $HOME_DIR/.conan2/profiles/
cp $ROOT_DIR/conan/ubuntu-gcc11-debug $HOME_DIR/.conan2/profiles/

conan install conanfile.py --build=missing --profile=ubuntu-gcc11-debug --profile:build=ubuntu-gcc11-release
conan install conanfile.py --build=missing --profile=ubuntu-gcc11-release --profile:build=ubuntu-gcc11-release

cmake --preset conan-debug
cmake --build build --preset conan-debug

cmake --preset conan-release
cmake --build build --preset conan-release