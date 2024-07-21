#!/bin/bash
virtualenv venv
source venv/bin/activate
pip install conan==2.5.0
conan --version