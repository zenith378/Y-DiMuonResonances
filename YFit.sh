#!/bin/sh

cmake -S . -B build
make -C build && ./build/main/YLaunch -m fit