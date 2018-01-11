//
// Created by kirill on 15.10.17.
//

#ifndef FPGA_UTILS_H
#define FPGA_UTILS_H


#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define FPGA_RUN 1

cl_context createContext(void);
cl_command_queue createCommandQueue(cl_context context, cl_device_id *device);
cl_program createProgram(cl_context context, cl_device_id device, const char *fileName);

// Print the error associciated with an error code
void printError(cl_int error);

void _checkError(int line,
                 const char *file,
                 cl_int error,
                 const char *msg,
                 ...);

#define checkError(status, ...) _checkError(__LINE__, __FILE__, status, __VA_ARGS__)

#endif //FPGA_UTILS_H
