#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include "CL/cl.h"
#endif
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <string.h>
// Sources:
// 1: Guide for C++: https://anteru.net/blog/2012/11/03/2009/
// 2: Guide for C: http://www.aronaldg.org/webfiles/compecon/src/opencl/doc/OpenCL_Mac_OS_X.pdf
// 3: OpenCL reference: https://www.khronos.org/registry/OpenCL/specs/opencl-1.1.pdf
// 4: example program: https://developer.apple.com/library/content/samplecode/OpenCL_Hello_World_Example/Listings/hello_c.html

// Error reporting function from: http://stackoverflow.com/questions/24326432/convenient-way-to-show-opencl-error-codes
const char *getErrorString(cl_int error) {
  switch(error) {
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
  }
}

cl_kernel kernel;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_device_id device_id;


void build_kernel() {
  
  // memory to hold device id

  cl_uint numDevices = 1; // Doesn't work for more than one yet.


  // ask OpenCL for one gpu
  // copied from 2
  cl_int err = clGetDeviceIDs(
			      NULL, // platform ID
			      CL_DEVICE_TYPE_GPU, // look only for GPUs
			      numDevices, // return an ID for only one GPU
			      &device_id, // on return, the device ID
			      NULL); // don't return the number of devices

  // make sure nothing went wrong
  if (err != CL_SUCCESS) {
    exit(1);
  }
  //  printf("Device id: %d\n", (int) device_id);



  // Copied from 2 with changes
  context = clCreateContext(
			     0, // a reserved variable
			     numDevices, // the number of devices in the devices parameter
			     &device_id, // a pointer to the list of device IDs from clGetDeviceIDs
			     NULL, // a pointer to an error notice callback function (if any) for runtime errors
			     NULL, // data to pass as a param to the callback function
			     &err); // on return, points to a result code

  if (err != CL_SUCCESS) {
    exit(1);
  }

  //  printf("Device context: %d\n", (int) context); // this is not supposed to make sense to the user
  
  // Copied from 2
  // The command queue for the first (and only) GPU
  commands = clCreateCommandQueue (
				    context, // a valid OpenCL context
				    device_id, // a device associated with the context
				    0, // a bit field to specify properties [1]
				    &err); // on return, points to a result code
  if (err != CL_SUCCESS) {
    exit(1);
  }



  // http://stackoverflow.com/questions/29121443/read-opencl-kernel-from-seperate-file
  FILE *kernel_file = fopen("sha1.cl", "rb");
  if (!kernel_file) {
    printf("Failed to open kernel file\n");
    exit(1);
  }
  
  fseek(kernel_file, 0, SEEK_END);
  size_t source_size = ftell(kernel_file);
  rewind(kernel_file);
  char *KernelSource = (char*)malloc(source_size + 1);
  KernelSource[source_size] = '\0';
  fread(KernelSource, sizeof(char), source_size, kernel_file);
  fclose(kernel_file);

  program = clCreateProgramWithSource (
					context, // a valid OpenCL context
					1, // the number of strings
					(const char**) &KernelSource, // an array of strings [1]
					NULL, // array of string lengths [2]
					&err // on return, points to a result code
					);
  
  if (err != CL_SUCCESS) {
    printf("Error: Failed to create program\n");
    exit(1);
  }

  // Copied from 2
  err = clBuildProgram(
		       program, // a valid program object
		       0, // number of devices in next parameter
		       NULL, // device list; NULL for all devices
		       //(const char*) &buildOptions, // a pointer to a string of build options
		       //NULL,
		       "-g",
		       NULL, // a pointer to a notification callback function
		       NULL // data to be passed as a parameter to the callback function
		       );
  
  if (err != CL_SUCCESS) {
    printf("Error: Failed to build program execuable\n\n");

    char buffer[2048];
    size_t len;
    
    // get the details on the error, and store it in buffer
    clGetProgramBuildInfo(
			  program, // the program object being queried
			  device_id, // the device for which the OpenCL code was built
			  CL_PROGRAM_BUILD_LOG, // specifies that we want the build log
			  sizeof(buffer), // the size of the buffer
			  buffer, // on return, holds the build log
			  &len); // on return, the actual size in bytes of the
    // data returned
    // print out the build errors
    printf("%s\n", buffer);

    exit(1);
    
  }

  free(KernelSource);
  
  const char* KernelName = "sha1_crypt_kernel";
  kernel = clCreateKernel(
			  program, // program to run
			   KernelName, // name of kernel
			   &err // error code
			   );

  if (err != CL_SUCCESS) {
    printf("Couldn't create kernel object: Error %d\n", err);
    exit(1);
  }



}





// Returns true / false depending on whether a preimage was found
int find_sha1(unsigned char* plain_key, const uint data_info[2], char* hash, char* hash_preimage) {

  cl_int err;
  
  size_t plain_key_size = sizeof(char) * data_info[1] * data_info[0];
  // digest is a uint array of len 5 * number of keys
  cl_uint* digest = calloc(data_info[0] * 5, sizeof(cl_uint));

  cl_mem data_info_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(uint) * 2, NULL, &err);
  //cl_mem salt_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, salt_size, NULL, &err);
  cl_mem plain_key_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, plain_key_size, NULL, &err);
  cl_mem digest_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * 5 * data_info[0], NULL, &err);

  
  // from 4
  if (!plain_key_buffer || !digest_buffer || !data_info_buffer) {
      printf("Error: Failed to allocate device memory!\n");
      exit(1);
  }


  err = clEnqueueWriteBuffer(
			     commands,
			     plain_key_buffer,
			     CL_TRUE, // blocking write
			     0, // zero offset
			     plain_key_size,
			     plain_key, // host mem source of data
			     0, // vvv a bunch of event stuff that doesn't matter vvv
			     NULL,
			     NULL);
  err |= clEnqueueWriteBuffer(commands, data_info_buffer, CL_TRUE, 0,
			      sizeof(uint) * 2, data_info, 0, NULL, NULL);
  /* err |= clEnqueueWriteBuffer(commands, salt_buffer, CL_TRUE, 0, */
  /* 			      salt_size, data_info, 0, NULL, NULL); */

  if (err != CL_SUCCESS) {
    printf("Couldn't copy host data to device: Error: %s\n", getErrorString(err));
    exit(1);
  }

  
  // From 4


  err = CL_SUCCESS;
  err |= clSetKernelArg(
   			kernel, // A kernel object
  			0, // argument index for kernel function
  			sizeof(cl_mem), // size of argument
  			&data_info_buffer); // Pointer to argument.

  //  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &salt_buffer);
  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &plain_key_buffer);
  err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &digest_buffer);

  if (err != CL_SUCCESS) {
    printf("Error: Failed to set kernel arguments! %s\n", getErrorString(err));
    exit(1);
  }
  
  // Get the maximum work group size for executing the kernel on the device
  // from 4
  size_t local; // local domain size
  size_t global; // global domain size
  err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
  if (err != CL_SUCCESS) {
    printf("Error: Failed to retrieve kernel work group info! %d\n", err);
    exit(1);
  }
  


  // Execute the kernel over the entire range of our 1d input data set
  // using the maximum number of work group items for this device
  // from 4

  
  global = data_info[0]; 
  err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
  if (err) {
    printf("Error: Failed to execute kernel!\n");
    exit(1);
  }
  


  // Wait for the command commands to get serviced before reading back results
  //
  clFinish(commands);
  
  // Read back the results from the device to verify the output
  //
  err = clEnqueueReadBuffer( commands, digest_buffer, CL_TRUE, 0, sizeof(cl_uint) * 5 * data_info[0], digest, 0, NULL, NULL );

  if (err != CL_SUCCESS) {
    printf("Error: Failed to read output array! %s\n", getErrorString(err));
    exit(1);
  }

  char current_hash[41];
  memset(current_hash, 0, sizeof(current_hash));


  //  printf("data_info[0]: %u\n", data_info[0]);
  int i;
  int j;
  for (i = 0; i < data_info[0]; i++) {
    sprintf(current_hash, "%08x%08x%08x%08x%08x", digest[5*i], digest[5*i + 1], digest[5*i + 2], digest[5*i + 3], digest[5*i + 4]);
    
    //printf("current hash: %s\t", current_hash);
    
    if (strcmp(current_hash, hash) == 0) {
      for (j = 0; j < data_info[1]; j++) {
	hash_preimage[j] = (char) plain_key[i * data_info[1] + j];
      }
      return true;
    }

    //debug
    for (j = 0; j < data_info[1]; j++) {
      hash_preimage[j] = (char) plain_key[i * data_info[1] + j];
    }
    //printf("Preimage: %s\n", hash_preimage);



  }
  
  //  printf("Output: %08x%08x%08x%08x%08x\n", digest[0], digest[1], digest[2], digest[3], digest[4]);


  
  
  // Shutdown and cleanup
  //
  clReleaseMemObject(plain_key_buffer);
  clReleaseMemObject(digest_buffer);
  clReleaseMemObject(data_info_buffer);

  return false;
}

void cleanup(char **line, FILE *fp) {
  free(*line);
  fclose(fp);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
}



int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: sha1 (file of keys) (hash)\n");
    exit(1);
  }

  FILE *fp = fopen(argv[1], "r");


  build_kernel();

  if (!fp) {
    printf("Failed to open file %s\n", argv[1]);
    exit(1);
  }

  
  char* hash = argv[2];

  int num_keys = pow(2, 8);
  // data_info[0] is the number of keys to process and data_info[1] is the size of each key
  const uint data_info[2] = {(uint) num_keys, 4};

  // plain_key is a string of keys which corresponds to data_info
  unsigned char plain_key[data_info[0] * data_info[1]];
  memset(plain_key, 0, sizeof(plain_key)); // zeroes the entries

  int i = 0;
  int j = 0;
  uint nextchar;
  char* line = (char *) calloc(data_info[1] + 1, sizeof(char));
  size_t len = data_info[1] * sizeof(char);
  int bytes_read;

  char hash_preimage[data_info[1] + 1]; // + null terminator
  memset(hash_preimage, 0, sizeof(hash_preimage));
  
  bytes_read = getline(&line, &len, fp);
  while (bytes_read > 0) {
    if (line[bytes_read - 1] == '\n') {
      line[(bytes_read - 1)] = '\0';
      bytes_read --;
      //printf("Removing newline. Bytes_Read is %d\n", bytes_read);
    }
    //printf("%s\n", line);
    for (j = 0; j < bytes_read; j++) {
      plain_key[i + j] = (unsigned char) line[j];
      //      printf("%c\n", plain_key[i + j]);
    }

    i += data_info[1];
    if ((i + data_info[1]) > data_info[0]) {
      
      if (find_sha1(plain_key, data_info, hash, hash_preimage)) {
	printf("sha1 inverse found: %s\n", hash_preimage);
	cleanup(&line, fp);
	exit(0);
      }
      i = 0;
    }
    //printf("%d\n", bytes_read);
    bytes_read = getline(&line, &len, fp);
  }

  // Try the last batch: incompletely filled.
  if (find_sha1(plain_key, data_info, hash, hash_preimage)) {
    printf("sha1 inverse found: %s\n", hash_preimage);
  }
  else {
    printf("sha1 inverse not found\n");
  }

  cleanup(&line, fp);
  exit(0);
}

