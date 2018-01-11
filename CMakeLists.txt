
cmake_minimum_required(VERSION 2.8)
project(hust-opencl)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(ALTERA_FPGA "Altera FPGA" ON)

if(ALTERA_FPGA)
    find_package(OpenCL)
    include_directories(${OpenCL_INCLUDE_DIRS})
    find_package(AlteraCL)
endif()

#find_package(BoardPackage)

#set(AOCLUtils ${CMAKE_CURRENT_SOURCE_/DIR}/aocl_common)
#include_directories(${AOCLUtils}/inc)

message("-- OpenCL_INCLUDE_DIRS: ${OpenCL_INCLUDE_DIRS}")
message("-- OpenCL_LIBRARIES: ${OpenCL_LIBRARIES}")
message("-- AlteraCL_LIBRARY: ${AlteraCL_LIBRARY}")

add_subdirectory(src)