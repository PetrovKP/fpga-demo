#!/bin/bash
export INTEL_FPGA_DIR=/home/lenferd/intelFPGA/17.1

export QUARTUS_ROOTDIR="$INTEL_FPGA_DIR"/quartus
export MODELSIM_ASE_DIR="$INTEL_FPGA_DIR"/modelsim_ase
export INTELFPGAOCLSDKROOT="$INTEL_FPGA_DIR"/hld
export ALTERAOCLSDKROOT="$INTELFPGAOCLSDKROOT"
export PATH=$PATH:"$QUARTUS_ROOTDIR"/bin:"$INTELFPGAOCLSDKROOT"/bin:"$MODELSIM_ASE_DIR"/bin
export AOCL_BOARD_PACKAGE_ROOT="$INTELFPGAOCLSDKROOT"/board/de10_nano
export LD_LIBRARY_PATH="$AOCL_BOARD_PACKAGE_ROOT"/linux64/lib

export pwd=$(pwd)
cd "$INTEL_FPGA_DIR"/embedded

#PATH="$INTEL_FPGA_DIR"/embedded/ds-5/bin:$PATH
#export PATH

source embedded_command_shell.sh

