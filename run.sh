#!/usr/bin/env bash

set -x

make -B && LD_LIBRARY_PATH=./thirdparty/raylib-6.0_linux_amd64/lib ./build/main
