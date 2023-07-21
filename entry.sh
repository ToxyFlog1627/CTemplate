#!/bin/sh
make -j8 DEBUG=${DEBUG:-1} && ./build/exe $@