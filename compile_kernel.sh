#!/bin/bash
# usage : source {this_file.sh}
export INTEL_FPGA_DIR=/home/lenferd/intelFPGA/17.1/

export QUARTUS_ROOTDIR="$INTEL_FPGA_DIR"/quartus
export MODELSIM_ASE_DIR="$INTEL_FPGA_DIR"/modelsim_ase
export INTELFPGAOCLSDKROOT="$INTEL_FPGA_DIR"/hld
export ALTERAOCLSDKROOT="$INTELFPGAOCLSDKROOT"
export PATH=$PATH:"$QUARTUS_ROOTDIR"/bin:"$INTELFPGAOCLSDKROOT"/linux64/bin:"$INTELFPGAOCLSDKROOT"/bin:"$MODELSIM_ASE_DIR"/bin
export AOCL_BOARD_PACKAGE_ROOT="$INTELFPGAOCLSDKROOT"/board/de10_nano
export LD_LIBRARY_PATH="$AOCL_BOARD_PACKAGE_ROOT"/linux64/lib:"$INTELFPGAOCLSDKROOT"/host/linux64/lib

export QUARTUS_64BIT=1

aoc src/saxpy.cl -o bin/saxpy.aocx -board=de10_nano_sharedonly -report
