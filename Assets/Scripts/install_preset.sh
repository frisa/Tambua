#!/bin/bash
preset=$1
root_dir=$(pwd)
home_dir=$(echo ~)

rm -fr $root_dir/build/$preset
#rm -fr ~/.conan2/p

rm -f $root_dir/.conan2/profiles/linux-release
rm -f $root_dir/.conan2/profiles/linux-debug

cp $root_dir/Assets/conan_profiles/linux-release $home_dir/.conan2/profiles/
cp $root_dir/Assets/conan_profiles/linux-debug $home_dir/.conan2/profiles/

conan --version
conan install conanfile.py --output-folder=./build/$preset --build=missing --profile=$preset --profile:build=linux-release -c tools.system.package_manager:mode=install
