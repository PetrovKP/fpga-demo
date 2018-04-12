## De10-nano soc demo (saxpy)

### *Warning*
All needed application (like Altera OpenCL SDK and DS5 sould be already installed)
Board should have installed on sd card bsp image

### Complile fpga kernel (aocx)
* Change INTEL_FPGA_DIR path in compile_kernel.sh
* $ source compile_kernel.sh


### Compile host application
* Change INTEL_FPGA_DIR path in env_host.sh
* $ source env_host.sh
* $ cd $pwd
* $ make (or make clean and then make)

Also check is DS5 installed in INTEL_FPGA_DIR/embedded/ds-5 directory
