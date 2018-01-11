#include <CL/cl.h>
#include "utils.h"

const unsigned int ARRAY_SIZE = 1024;

int main(int argc, char **argv) {
  cl_context context = 0;
  cl_command_queue commandQueue = 0;
  cl_program program = 0;
  cl_device_id device = 0;

  cl_kernel kernel = 0;
  cl_int err;

  context = createContext();

  commandQueue = createCommandQueue(context, &device);

  program = createProgram(context, device, "saxpy.aocx");
  kernel = clCreateKernel(program, "saxpy", NULL);

  double x[ARRAY_SIZE], y[ARRAY_SIZE];
  for (int i = 0; i < ARRAY_SIZE; i++) {
    x[i] = i*1.0;
    y[i] = i*2.0;
  }
  const double a = 1.0;

  cl_mem memX = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*ARRAY_SIZE, x, &err);
  if (err != CL_SUCCESS) {
    checkError(err, "clCreateBuffer x");
  }

  cl_mem memY = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*ARRAY_SIZE, y, &err);
  if (err != CL_SUCCESS) {
    checkError(err, "clCreateBuffer y");
  }

  err = clSetKernelArg(kernel, 0, sizeof(unsigned int), &ARRAY_SIZE);
  err |= clSetKernelArg(kernel, 1, sizeof(double), &a);
  err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memX);
  err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &memY);

  if (err != CL_SUCCESS) {
    checkError(err, "clSetKernelArg");
  }

  size_t globalWorkSize[1] = {ARRAY_SIZE};
  size_t localWorkSize[1] = {1};

  err = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

  if (err != CL_SUCCESS) {
    checkError(err, "clEnqueueNDRangeKernel");
  }

  err = clEnqueueReadBuffer(commandQueue, memY, CL_TRUE, 0, ARRAY_SIZE * sizeof(float), y, 0, NULL, NULL);

  if (err != CL_SUCCESS) {
    checkError(err, "clEnqueueReadBuffer");
  }

//  for (int i = 0; i < ARRAY_SIZE; i++) {
//    printf("%lf ", y[i]);
//  }
}