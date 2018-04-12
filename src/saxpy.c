#include <CL/cl.h>
#include "utils.h"

const unsigned int ARRAY_SIZE = 1024;

int main(int argc, char **argv) {

  printf("Program was started\n");

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
    y[i] = i*1.0;
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

  double result[ARRAY_SIZE];
  err = clEnqueueReadBuffer(commandQueue, memY, CL_TRUE, 0, ARRAY_SIZE * sizeof(double), result, 0, NULL, NULL);

  if (err != CL_SUCCESS) {
    checkError(err, "clEnqueueReadBuffer");
  }

  double max_fault = 1e-2;
  int errors = 0;
  for (size_t i = 0; i < ARRAY_SIZE; i++) {
    double reference = y[i] + a * x[i];
    // printf("%lf | %lf\n", result[i], reference);
    if (result[i] - reference > max_fault) {
      fprintf(stderr, "Values not equal: result %lf != %lf reference\n", result[i], reference);
      ++errors;
    }
  }

  if (errors) {
    printf("Got %d errors\n", errors);
  } else {
    printf("Programm was complete without errors\n");
  }

  return 0;
}
