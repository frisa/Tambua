#!/bin/bash
virtualenv conan2
source conan2/bin/activate
pip install conan==2.5.0
conan --version