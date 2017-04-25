#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include "CL/cl.h"
#endif
#include <stdio.h>
#define BUFFER_SIZE (4096)
// Sources:
// 1: Guide for C++: https://anteru.net/blog/2012/11/03/2009/
// 2: Guide for C: http://www.aronaldg.org/webfiles/compecon/src/opencl/doc/OpenCL_Mac_OS_X.pdf
// 3: OpenCL reference: https://www.khronos.org/registry/OpenCL/specs/opencl-1.1.pdf
// 4: example program: https://developer.apple.com/library/content/samplecode/OpenCL_Hello_World_Example/Listings/hello_c.html

int main(int argc, char **argv) {
  // memory to hold device id
  cl_device_id device_id;
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
  cl_context context = clCreateContext(
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
  cl_command_queue commands = clCreateCommandQueue (
					  context, // a valid OpenCL context
					  device_id, // a device associated with the context
					  0, // a bit field to specify properties [1]
					  &err); // on return, points to a result code
  if (err != CL_SUCCESS) {
    exit(1);
  }



  // http://stackoverflow.com/questions/29121443/read-opencl-kernel-from-seperate-file
  FILE *fp = fopen("kernel.cl", "rb");
  if (!fp) {
    printf("Failed to open kernel file\n");
    exit(1);
  }
  
  fseek(fp, 0, SEEK_END);
  size_t source_size = ftell(fp);
  rewind(fp);
  char *KernelSource = (char*)malloc(source_size + 1);
  KernelSource[source_size] = '\0';
  fread(KernelSource, sizeof(char), source_size, fp);
  fclose(fp);

  // Copied from 2
  // OpenCL kernel that computes the square of an input array
  /* const char *KernelSource = "\n"		\ */
  /*   "__kernel void square( \n"			\ */
  /*   " __global float* input, \n"		\ */
  /*   " __global float* output, \n"		\ */
  /*   " const unsigned int count) \n"		\ */
  /*   "{ \n"					\ */
  /*   " int i = get_global_id(0); \n"		\ */
  /*   " if(i < count) \n"				\ */
  /*   " output[i] = input[i] * input[i]; \n"	\ */
  /*   "} \n"					\ */
  /*   "\n"; */
  cl_program program = clCreateProgramWithSource (
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
		       NULL,
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

  


  
  const char* KernelName = "square";
  cl_kernel kernel = clCreateKernel(
				    program, // program to run
				    KernelName, // name of kernel
				    &err // error code
				    );

  if (err != CL_SUCCESS) {
    printf("Kernel faied\n");
    exit(1);
  }

  const unsigned int count = BUFFER_SIZE;
  const size_t buffer_size = sizeof(float) * count;
  float input_buffer[count];
  int i;
  for (i = 0; i < count; i++) {
    input_buffer[i] = rand() / (float)RAND_MAX;
  }
  float output_buffer[count];

  //  int work_items = 10; // no limit to this. Set it to len of buffer
  // work-group size is the size of the group of items to process. Probably set to null and let opencl deal
  // with it since this problem doesn't require shared memory


  cl_mem input = clCreateBuffer(
				 context,
				 CL_MEM_READ_ONLY,
				 // memory flags: read only
				 buffer_size,
				 NULL, // don't initialize from host stuff
				 &err // errors
				 );

  cl_mem output = clCreateBuffer(
				 context,
				 CL_MEM_WRITE_ONLY,
				 // memory flags: Write only
				 buffer_size,
				 NULL, // don't initialize from host stuff
				 &err // errors
				 );

  // from 4
  if (!input || !output) {
      printf("Error: Failed to allocate device memory!\n");
      exit(1);
  }
  
  // copy to device memory
  err = clEnqueueWriteBuffer(
			     commands,
			     input,
			     CL_TRUE, // blocking write
			     0, // zero offset
			     buffer_size,
			     input_buffer, // host mem source of data
			     0, // vvv a bunch of event stuff that doesn't matter vvv
			     NULL,
			     NULL);

  if (err != CL_SUCCESS) {
    printf("Couldn't copy host data to device\n");
    exit(1);
  }

  // From 4
  err = CL_SUCCESS;
  err |= clSetKernelArg(
   			kernel, // A kernel object
			0, // argument index for kernel function
			sizeof(cl_mem), // size of argument
			&input); // Pointer to argument.

  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
  err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);

  if (err != CL_SUCCESS) {
    printf("Error: Failed to set kernel arguments! %d\n", err);
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
  global = count;
  err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
  if (err) {
    printf("Error: Failed to execute kernel!\n");
    return EXIT_FAILURE;
  }
  
  // Wait for the command commands to get serviced before reading back results
  //
  clFinish(commands);
  
  // Read back the results from the device to verify the output
  //
  err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * count, output_buffer, 0, NULL, NULL );  
  if (err != CL_SUCCESS) {
    printf("Error: Failed to read output array! %d\n", err);
    exit(1);
  }
  
  // Validate our results
  //
  int correct = 0;
  for(i = 0; i < count; i++) {
    //    printf("%f ", output_buffer[i]);
      if(output_buffer[i] == input_buffer[i] * input_buffer[i])
	correct++;
  }
  
  // Print a brief summary detailing the results
  //
  printf("Computed '%d/%d' correct values!\n", correct, count);
  
  // Shutdown and cleanup
  //
  clReleaseMemObject(input);
  clReleaseMemObject(output);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
  
  return 0;
}
