//
// Created by kirill on 15.10.17.
//


#include "utils.h"

cl_context createContext(void) {
//  выбрана первая платформа
  const cl_int platform = 0;

  cl_int errNum;
  cl_uint numPlatforms;

  cl_context context = NULL;

//  Получение числа платформ
  errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
  checkError(errNum, "Failed to find any OpenCL platforms.");


//  Получения num доступных платформ
  cl_platform_id* platformsId = (cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);
  errNum = clGetPlatformIDs(numPlatforms, platformsId, NULL);
  checkError(errNum, "clGetPlatformIDs");

//  Получение характеристики платформы (версия платофрмы)
  char platformName[100];
  errNum = clGetPlatformInfo(platformsId[platform], CL_PLATFORM_NAME, sizeof(platformName),platformName, NULL);
  checkError(errNum, "Info?");
  fprintf(stderr, "Platform: %s\n", platformName);


//  Создание контекста для управления объектами OpenCL
  cl_context_properties contextProperties[] = {
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)platformsId[platform],
      0
  };

  context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_ACCELERATOR, NULL, NULL, &errNum);
  checkError(errNum, "Could not create GPU context, trying CPU..");

  if (errNum != CL_SUCCESS) {
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
    checkError(errNum, "Could not create CPU context");
  }

  return context;
}

cl_command_queue createCommandQueue(cl_context context, cl_device_id *device) {
  cl_int errNum;
  cl_device_id *devices;
  cl_command_queue commandQueue = NULL;
  size_t deviceBufferSize = 0;

  // get devices number
  errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
  checkError(errNum, "Failed call to clGetContextInfo(..., CL_CONTEXT_DEVICES, ...)");
//  printf("device buffer size %lu\n", deviceBufferSize);
  devices = (cl_device_id*)malloc(deviceBufferSize);

  errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
  checkError(errNum, "Failed to get device IDs");

  commandQueue = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE, NULL);
  if (commandQueue == NULL) {
    fprintf(stderr, "Failed to create commandQueue for device 0.\n");
    exit(-1);
  }

  *device = devices[0];
  free(devices);
  return commandQueue;
}

cl_program createProgram(cl_context context, cl_device_id device, const char *fileName) {
  cl_int errNum;
  const char* kernel_file_mode;

  kernel_file_mode = "rb";

  FILE* kernel_fp = fopen(fileName, kernel_file_mode);
  if(kernel_fp == NULL){
    fprintf(stderr,"common_ocl.ocdBuildProgramFromFile() - Cannot open kernel file!");
    exit(-1);
  }
  fseek(kernel_fp, 0, SEEK_END);
  size_t kernelLength = (size_t) ftell(kernel_fp);
  char* kernelSource = (char *)malloc(sizeof(char)*kernelLength);
  if(kernelSource == NULL){
    fprintf(stderr,"common_ocl.ocdBuildProgramFromFile() - Heap Overflow! Cannot allocate space for kernelSource.");
    exit(-1);
  }
  rewind(kernel_fp);
  size_t items_read = fread((void *) kernelSource, kernelLength, 1, kernel_fp);
  if(items_read != 1){
    fprintf(stderr,"common_ocl.ocdBuildProgramFromFile() - Error reading from kernelFile");
    exit(-1);
  }
  fclose(kernel_fp);

  /* Create the compute program from the source buffer */
  cl_program program;
  //use Altera FPGA
  program = clCreateProgramWithBinary(context,
                                       1,
                                       &device,
                                       &kernelLength,
                                       (const unsigned char **) &kernelSource,
                                       NULL,
                                       &errNum);
  checkError(errNum, "common_ocl.ocdBuildProgramFromFile() - Failed to create a compute program!");

  /* Build the program executable */
  //use Altera FPGA
  errNum = clBuildProgram(program, 1, &device, "-DOPENCL -I.", NULL, NULL);

  if (errNum == CL_BUILD_PROGRAM_FAILURE)
  {
    char *buildLog;
    size_t logLen;
    errNum = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logLen);
    buildLog = (char *) malloc(sizeof(char)*logLen);
    if(buildLog == NULL){
      fprintf(stderr,"common_ocl.ocdBuildProgramFromFile() - Heap Overflow! Cannot allocate space for buildLog.");
      exit(-1);
    }
    errNum = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logLen, (void *) buildLog, NULL);
    fprintf(stderr, "CL Error %d: Failed to build program! Log:\n%s", errNum, buildLog);
    free(buildLog);
    exit(1);
  }
  checkError(errNum,"common_ocl.ocdBuildProgramFromFile() - Failed to build program!");

  free(kernelSource);
  return program;
}

void printError(cl_int error) {
  // Print error message
  switch(error)
  {
    case -1:
      printf("CL_DEVICE_NOT_FOUND ");
      break;
    case -2:
      printf("CL_DEVICE_NOT_AVAILABLE ");
      break;
    case -3:
      printf("CL_COMPILER_NOT_AVAILABLE ");
      break;
    case -4:
      printf("CL_MEM_OBJECT_ALLOCATION_FAILURE ");
      break;
    case -5:
      printf("CL_OUT_OF_RESOURCES ");
      break;
    case -6:
      printf("CL_OUT_OF_HOST_MEMORY ");
      break;
    case -7:
      printf("CL_PROFILING_INFO_NOT_AVAILABLE ");
      break;
    case -8:
      printf("CL_MEM_COPY_OVERLAP ");
      break;
    case -9:
      printf("CL_IMAGE_FORMAT_MISMATCH ");
      break;
    case -10:
      printf("CL_IMAGE_FORMAT_NOT_SUPPORTED ");
      break;
    case -11:
      printf("CL_BUILD_PROGRAM_FAILURE ");
      break;
    case -12:
      printf("CL_MAP_FAILURE ");
      break;
    case -13:
      printf("CL_MISALIGNED_SUB_BUFFER_OFFSET ");
      break;
    case -14:
      printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST ");
      break;
    case -30:
      printf("CL_INVALID_VALUE ");
      break;
    case -31:
      printf("CL_INVALID_DEVICE_TYPE ");
      break;
    case -32:
      printf("CL_INVALID_PLATFORM ");
      break;
    case -33:
      printf("CL_INVALID_DEVICE ");
      break;
    case -34:
      printf("CL_INVALID_CONTEXT ");
      break;
    case -35:
      printf("CL_INVALID_QUEUE_PROPERTIES ");
      break;
    case -36:
      printf("CL_INVALID_COMMAND_QUEUE ");
      break;
    case -37:
      printf("CL_INVALID_HOST_PTR ");
      break;
    case -38:
      printf("CL_INVALID_MEM_OBJECT ");
      break;
    case -39:
      printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR ");
      break;
    case -40:
      printf("CL_INVALID_IMAGE_SIZE ");
      break;
    case -41:
      printf("CL_INVALID_SAMPLER ");
      break;
    case -42:
      printf("CL_INVALID_BINARY ");
      break;
    case -43:
      printf("CL_INVALID_BUILD_OPTIONS ");
      break;
    case -44:
      printf("CL_INVALID_PROGRAM ");
      break;
    case -45:
      printf("CL_INVALID_PROGRAM_EXECUTABLE ");
      break;
    case -46:
      printf("CL_INVALID_KERNEL_NAME ");
      break;
    case -47:
      printf("CL_INVALID_KERNEL_DEFINITION ");
      break;
    case -48:
      printf("CL_INVALID_KERNEL ");
      break;
    case -49:
      printf("CL_INVALID_ARG_INDEX ");
      break;
    case -50:
      printf("CL_INVALID_ARG_VALUE ");
      break;
    case -51:
      printf("CL_INVALID_ARG_SIZE ");
      break;
    case -52:
      printf("CL_INVALID_KERNEL_ARGS ");
      break;
    case -53:
      printf("CL_INVALID_WORK_DIMENSION ");
      break;
    case -54:
      printf("CL_INVALID_WORK_GROUP_SIZE ");
      break;
    case -55:
      printf("CL_INVALID_WORK_ITEM_SIZE ");
      break;
    case -56:
      printf("CL_INVALID_GLOBAL_OFFSET ");
      break;
    case -57:
      printf("CL_INVALID_EVENT_WAIT_LIST ");
      break;
    case -58:
      printf("CL_INVALID_EVENT ");
      break;
    case -59:
      printf("CL_INVALID_OPERATION ");
      break;
    case -60:
      printf("CL_INVALID_GL_OBJECT ");
      break;
    case -61:
      printf("CL_INVALID_BUFFER_SIZE ");
      break;
    case -62:
      printf("CL_INVALID_MIP_LEVEL ");
      break;
    case -63:
      printf("CL_INVALID_GLOBAL_WORK_SIZE ");
      break;
    default:
      printf("UNRECOGNIZED ERROR CODE (%d)", error);
  }
}

void _checkError(int line,
                 const char *file,
                 cl_int error,
                 const char *msg,
                 ...) {
  // If not successful
  if(error != CL_SUCCESS) {
    // Print line and file
    printf("ERROR: ");
    printError(error);
    printf("\nLocation: %s:%d\n", file, line);

    // Print custom message.
    va_list vl;
    va_start(vl, msg);
    vprintf(msg, vl);
    printf("\n");
    va_end(vl);

    exit(error);
  }
}