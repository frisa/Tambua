#!/bin/bash
preset=$1
cmake --preset $preset
cmake --build --preset $preset
