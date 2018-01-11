#!/usr/bin/env bash
export ALTERAOCLSDKROOT=/opt/intelFPGA_pro/17.0/hld
export PATH=$PATH:$ALTERAOCLSDKROOT/bin
export AOCL_BOARD_PACKAGE_ROOT=/opt/intelFPGA_pro/17.0/hld/board/a10soc
export LD_LIBRARY_PATH=$AOCL_BOARD_PACKAGE_ROOT/linux64/lib:$ALTERAOCLSDKROOT/linux64/lib:$ALTERAOCLSDKROOT/host/linux64/lib

mkdir _build
cd _build
cmake  ..
make
cd src
aoc -march=emulator saxpy.cl -o ./saxpy.aocx --board a10soc --profile
CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1 ./axpy
