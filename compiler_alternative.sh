#!/bin/bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 13
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 13
sudo update-alternatives --config gcc
sudo update-alternatives --config g++
gcc --version
g++ --version