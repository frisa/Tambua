#!/bin/bash
preset=$1
root_dir=$(pwd)
home_dir=$(echo ~)

if ( [ "$preset" != "linux-release" ] && [ "$preset" != "linux-debug" ] ); then
    echo "Invalid preset: $preset"
    exit 1
fi

rm -fr $root_dir/build/$preset
#rm -fr ~/.conan2/p

rm -f $root_dir/.conan2/profiles/linux-release
rm -f $root_dir/.conan2/profiles/linux-debug

cp $root_dir/Data/conan_profiles/linux-release $home_dir/.conan2/profiles/
cp $root_dir/Data/conan_profiles/linux-debug $home_dir/.conan2/profiles/

conan --version
conan install conanfile.py --output-folder=./build/$preset --build=missing --profile=$preset --profile:build=linux-release -c tools.system.package_manager:mode=install
