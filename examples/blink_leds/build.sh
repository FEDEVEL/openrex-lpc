#!/usr/bin/env bash

# Script uses 'cmake' to generate makefiles and then runs 'make' to build 
# application. Content of build directory is always removed to avoid
# magic issues with cmake cache. If you want to use it in real life, please
# separate 'generate' and 'build' process so 'make' can use advantages of 
# dependency files 

# export CROSS_COMPILE="/home/ubuntu/bin/gnuarm/bin/arm-none-eabi-"
if [ ! -v CROSS_COMPILE ]; then
	echo ""
	echo "Please set CROSS_COMPILE variable before executing 'build.sh' script "
	echo "for instance: 'export CROSS_COMPILE=\"/home/ubuntu/bin/gnuarm/bin/arm-none-eabi-\"'"
	echo ""
	return -1
fi
rm -rf build/*
mkdir -p build
cmake -B"./build" -H"./" -G "Unix Makefiles"
(cd build && make VERBOSE=1)
