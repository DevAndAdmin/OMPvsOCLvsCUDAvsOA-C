#include "pch.h"
#include <windows.h>
#include <iostream>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <omp.h>
#include <conio.h>


#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif 
#define MAX_SOURCE_SIZE (0x100000)

const unsigned int WIDTH = 4000;
const unsigned int HEIGHT = 4000;
const unsigned int MAX_ITERS = 100;
const unsigned int MAX_COLOR = 255;
const double xmin = -1.7;
const double xmax = .5;
const double ymin = -1.2;
const double ymax = 1.2;
const double dx = (xmax - xmin) / WIDTH;
const double dy = (ymax - ymin) / HEIGHT;


//Вывод данных о device
void clPrintDevInfo(cl_device_id device) {
	char device_string[1024];
/*
	// CL_DEVICE_NAME
	clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_string), &device_string, NULL);
	printf("  CL_DEVICE_NAME: \t\t\t%s\n", device_string);

	 CL_DEVICE_VENDOR
	clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(device_string), &device_string, NULL);
	printf("  CL_DEVICE_VENDOR:  \t\t\t%s\n", device_string);

	 CL_DRIVER_VERSION
	clGetDeviceInfo(device, CL_DRIVER_VERSION, sizeof(device_string), &device_string, NULL);
	printf("  CL_DRIVER_VERSION: \t\t\t%s\n", device_string);

	 CL_DEVICE_INFO
	cl_device_type type;
	clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(type), &type, NULL);
	if (type & CL_DEVICE_TYPE_CPU)
		printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_CPU");
	if (type & CL_DEVICE_TYPE_GPU)
		printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_GPU");
	if (type & CL_DEVICE_TYPE_ACCELERATOR)
		printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_ACCELERATOR");
	if (type & CL_DEVICE_TYPE_DEFAULT)
		printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_DEFAULT");
*/
	// CL_DEVICE_MAX_COMPUTE_UNITS
	cl_uint compute_units;
	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);
	printf("  CL_DEVICE_MAX_COMPUTE_UNITS:\t\t%u\n", compute_units);

	// CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
	size_t workitem_dims;
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(workitem_dims), &workitem_dims, NULL);
	printf("  CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:\t%u\n", workitem_dims);

	// CL_DEVICE_MAX_WORK_ITEM_SIZES
	size_t workitem_size[3];
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workitem_size), &workitem_size, NULL);
	printf("  CL_DEVICE_MAX_WORK_ITEM_SIZES:\t%u / %u / %u \n", workitem_size[0], workitem_size[1], workitem_size[2]);

	// CL_DEVICE_MAX_WORK_GROUP_SIZE
	size_t workgroup_size;
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(workgroup_size), &workgroup_size, NULL);
	printf("  CL_DEVICE_MAX_WORK_GROUP_SIZE:\t%u\n", workgroup_size);

	// CL_DEVICE_MAX_CLOCK_FREQUENCY
	//cl_uint clock_frequency;
	//clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency), &clock_frequency, NULL);
	//printf("  CL_DEVICE_MAX_CLOCK_FREQUENCY:\t%u MHz\n", clock_frequency);

	// CL_DEVICE_ADDRESS_BITS
	//cl_uint addr_bits;
	//clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, sizeof(addr_bits), &addr_bits, NULL);
	//printf("  CL_DEVICE_ADDRESS_BITS:\t\t%u\n", addr_bits);

	// CL_DEVICE_MAX_MEM_ALLOC_SIZE
	//cl_ulong max_mem_alloc_size;
	//clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(max_mem_alloc_size), &max_mem_alloc_size, NULL);
	//printf("  CL_DEVICE_MAX_MEM_ALLOC_SIZE:\t\t%u MByte\n", (unsigned int)(max_mem_alloc_size / (1024 * 1024)));

	// CL_DEVICE_GLOBAL_MEM_SIZE
	//cl_ulong mem_size;
	//clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
	//printf("  CL_DEVICE_GLOBAL_MEM_SIZE:\t\t%u MByte\n", (unsigned int)(mem_size / (1024 * 1024)));

	// CL_DEVICE_ERROR_CORRECTION_SUPPORT
	//cl_bool error_correction_support;
	//clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(error_correction_support), &error_correction_support, NULL);
	//printf("  CL_DEVICE_ERROR_CORRECTION_SUPPORT:\t%s\n", error_correction_support == CL_TRUE ? "yes" : "no");

	// CL_DEVICE_LOCAL_MEM_TYPE
	//cl_device_local_mem_type local_mem_type;
	//clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(local_mem_type), &local_mem_type, NULL);
	//printf("  CL_DEVICE_LOCAL_MEM_TYPE:\t\t%s\n", local_mem_type == 1 ? "local" : "global");

	// CL_DEVICE_LOCAL_MEM_SIZE
	//clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
	//printf("  CL_DEVICE_LOCAL_MEM_SIZE:\t\t%u KByte\n", (unsigned int)(mem_size / 1024));

	// CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
	//clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(mem_size), &mem_size, NULL);
	//printf("  CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:\t%u KByte\n", (unsigned int)(mem_size / 1024));

	// CL_DEVICE_QUEUE_PROPERTIES
	//cl_command_queue_properties queue_properties;
	//clGetDeviceInfo(device, CL_DEVICE_QUEUE_PROPERTIES, sizeof(queue_properties), &queue_properties, NULL);
	//if (queue_properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
		//printf("  CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE");
	//if (queue_properties & CL_QUEUE_PROFILING_ENABLE)
		//printf("  CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_PROFILING_ENABLE");

	// CL_DEVICE_IMAGE_SUPPORT
	//cl_bool image_support;
	//clGetDeviceInfo(device, CL_DEVICE_IMAGE_SUPPORT, sizeof(image_support), &image_support, NULL);
	//printf("  CL_DEVICE_IMAGE_SUPPORT:\t\t%u\n", image_support);

	// CL_DEVICE_MAX_READ_IMAGE_ARGS
	//cl_uint max_read_image_args;
	//clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(max_read_image_args), &max_read_image_args, NULL);
	//printf("  CL_DEVICE_MAX_READ_IMAGE_ARGS:\t%u\n", max_read_image_args);

	// CL_DEVICE_MAX_WRITE_IMAGE_ARGS
	//cl_uint max_write_image_args;
	//clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(max_write_image_args), &max_write_image_args, NULL);
	//printf("  CL_DEVICE_MAX_WRITE_IMAGE_ARGS:\t%u\n", max_write_image_args);

	// CL_DEVICE_IMAGE2D_MAX_WIDTH, CL_DEVICE_IMAGE2D_MAX_HEIGHT, CL_DEVICE_IMAGE3D_MAX_WIDTH, CL_DEVICE_IMAGE3D_MAX_HEIGHT, CL_DEVICE_IMAGE3D_MAX_DEPTH
	//size_t szMaxDims[5];
	//printf("\n  CL_DEVICE_IMAGE <dim>");
	//clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &szMaxDims[0], NULL);
	//printf("\t\t\t2D_MAX_WIDTH\t %u\n", szMaxDims[0]);
	//clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t), &szMaxDims[1], NULL);
	//printf("\t\t\t\t\t2D_MAX_HEIGHT\t %u\n", szMaxDims[1]);
	//clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t), &szMaxDims[2], NULL);
	//printf("\t\t\t\t\t3D_MAX_WIDTH\t %u\n", szMaxDims[2]);
	//clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t), &szMaxDims[3], NULL);
	//printf("\t\t\t\t\t3D_MAX_HEIGHT\t %u\n", szMaxDims[3]);
	//clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t), &szMaxDims[4], NULL);
	//printf("\t\t\t\t\t3D_MAX_DEPTH\t %u\n", szMaxDims[4]);

	// CL_DEVICE_PREFERRED_VECTOR_WIDTH_<type>
	//printf("  CL_DEVICE_PREFERRED_VECTOR_WIDTH_<t>\t");
	//cl_uint vec_width[6];
	//clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(cl_uint), &vec_width[0], NULL);
	//clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(cl_uint), &vec_width[1], NULL);
	//clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(cl_uint), &vec_width[2], NULL);
	//clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(cl_uint), &vec_width[3], NULL);
	//clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cl_uint), &vec_width[4], NULL);
	//clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint), &vec_width[5], NULL);
	//printf("CHAR %u, SHORT %u, INT %u, FLOAT %u, DOUBLE %u\n\n\n",vec_width[0], vec_width[1], vec_width[2], vec_width[3], vec_width[4]);
}

int main()
{
	//переменные
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	cl_platform_id clSelectedPlatformID = NULL;
	cl_device_id *devices;
	SYSTEMTIME st;
	double st1, st2;
	char cBuffer[1024];
	double x0, y0, x1, y1, xtemp;
	int i,y,x;

	
	cl_uint platformIdCount = 0;
	
	//-------------------------------------------------------------- переменные --------------------------------------------------------------------------------------------
	printf("Start\n"); 

	// Create the two input vectors
	/*int i;
	const int LIST_SIZE = 999999;
	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
	for (i = 0; i < LIST_SIZE; i++) {
		A[i] = i;
		B[i] = LIST_SIZE - i;
	}*/
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	//--------------------------------------------------------------- Инициализация общих переменных ----------------------------------------------------------------------------------
	size_t bytes = WIDTH * HEIGHT * sizeof(unsigned int);
	unsigned char *image = (unsigned char*)malloc(bytes);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//NO PARALLEL
	GetLocalTime(&st);
	printf("standart code started: %d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	st1 = omp_get_wtime();
	//code
	//*image = (unsigned char*)malloc(bytes);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			x0 = xmin + x * dx;
			y0 = ymin + y * dy;
			x1 = 0.0;
			y1 = 0.0;
			for (i = 0; x1*x1 + y1 * y1 < 4.0 && i < MAX_ITERS; i++) {
				double xtemp = x1 * x1 - y1 * y1 + x0;
				y1 = 2 * x1*y1 + y0;
				x1 = xtemp;
			}
			image[y*WIDTH + x] = (double)MAX_COLOR*i / MAX_ITERS;
		}
	}
	//
	st2 = omp_get_wtime();
	GetLocalTime(&st);
	printf("standart code finished: %d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("%f seconds.\n", st2 - st1); 
	
	//------------------------------------------------------ Пишем результат в файл ----------------------------------------------------------------------------------
	FILE *fp = fopen("mandelbrot_standart.pgm", "wb");
	fprintf(fp, "P5\n%s\n%d %d\n%d\n", "#comment", WIDTH, HEIGHT, MAX_COLOR);
	fwrite(image, sizeof(unsigned char), WIDTH*HEIGHT, fp);
	fclose(fp);
	free(image);
	//

	//Записать результат в файл
	/*FILE *f;
	f = fopen("standart.txt", "w");
	if (!f) {
		fprintf(stderr, "Failed to write results for standart code.\n");
	}
	else {
		for (i = 0; i < LIST_SIZE; i++) fprintf(f, " %d", C[i]);
		fclose(f);
	}
	*/
	//NO PARALLEL END
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//OPENMP
	printf_s("CPU Cores: %d\n", omp_get_num_procs());
	GetLocalTime(&st);
	printf("OpenMP code started: %d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	st1 = omp_get_wtime();
	//code
	image = (unsigned char*)malloc(bytes);
	#pragma omp parallel for private(y, x, i, x0, y0, x1, y1, xtemp) //num_threads(n) задать кол-во потоков
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			x0 = xmin + x * dx;
			y0 = ymin + y * dy;
			x1 = 0.0;
			y1 = 0.0;
			for (i = 0; x1*x1 + y1 * y1 < 4.0 && i < MAX_ITERS; i++) {
				xtemp = x1 * x1 - y1 * y1 + x0;
				y1 = 2 * x1*y1 + y0;
				x1 = xtemp;
			}
			image[y*WIDTH + x] = (double)MAX_COLOR*i / MAX_ITERS;
		}
	}
	//
	st2 = omp_get_wtime();
	GetLocalTime(&st);
	printf("OpenMP code finished: %d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("%f seconds.\n", st2 - st1);
	fp = fopen("mandelbrot_omp.pgm", "wb");
	fprintf(fp, "P5\n%s\n%d %d\n%d\n", "#comment", WIDTH, HEIGHT, MAX_COLOR);
	fwrite(image, sizeof(unsigned char), WIDTH*HEIGHT, fp);
	fclose(fp);
	free(image);
	//OPENMP END

/*
	//OPENCL
		// Грузим kernel в массив source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("kernel.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	printf("kernel loading done\n");

	// Get platform and device information
	ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platforms * sizeof(cl_platform_id));
	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	printf("%d OpenCL Platforms found\n", ret_num_platforms);
	for (cl_uint j = 0; j < ret_num_platforms; ++j) {
		//clSelectedPlatformID = platforms[j];
		ret = clGetPlatformInfo(platforms[j], CL_PLATFORM_NAME, sizeof(cBuffer), &cBuffer, NULL);
		printf("\nCL_PLATFORM_NAME (VERSION): \t%s", cBuffer);
		ret = clGetPlatformInfo(platforms[j], CL_PLATFORM_VERSION, sizeof(cBuffer), &cBuffer, NULL);
		printf(" (%s,)", cBuffer);
		ret = clGetDeviceIDs(platforms[j], CL_DEVICE_TYPE_ALL, 1, &device_id, &ret_num_devices);
		ret = clGetPlatformInfo(platforms[j], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
		printf(" %u devices found supporting OpenCL\n\n", ret_num_devices, cBuffer);
		if ((devices = (cl_device_id*)malloc(sizeof(cl_device_id) * ret_num_devices)) == NULL) {
			printf(" Failed to allocate memory for devices !!!\n\n");
		}
		ret = clGetDeviceIDs(platforms[j], CL_DEVICE_TYPE_ALL, ret_num_devices, devices, &ret_num_devices);
		if (ret == CL_SUCCESS) {
			for (unsigned int v = 0; v < ret_num_devices; ++v) {
				clGetDeviceInfo(devices[v], CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
				printf("\033[01;35m Device %s\033[01;37m\n", cBuffer);
				clPrintDevInfo(devices[v]);
				// Create an OpenCL context
				//context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
				context = clCreateContext(NULL, 1, &devices[v], NULL, NULL, &ret);
				// Create a command queue
				command_queue = clCreateCommandQueue(context, devices[v], 0, &ret);
				// Create memory buffers on the device for each vector 
				cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
					LIST_SIZE * sizeof(int), NULL, &ret);
				cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
					LIST_SIZE * sizeof(int), NULL, &ret);
				cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
					LIST_SIZE * sizeof(int), NULL, &ret);
				// Copy the lists A and B to their respective memory buffers
				ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
					LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
				ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
					LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
				// Create a program from the kernel source
				program = clCreateProgramWithSource(context, 1,
					(const char **)&source_str, (const size_t *)&source_size, &ret);
				// Build the program
				ret = clBuildProgram(program, 1, &devices[v], NULL, NULL, NULL);
				// Create the OpenCL kernel
				kernel = clCreateKernel(program, "vector_add", &ret);
				// Set the arguments of the kernel
				ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
				ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
				ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
				//added this to fix garbage output problem
				//ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);
				st1 = omp_get_wtime();
				GetLocalTime(&st);
				printf("Opencl compute started: %d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
				// Execute the OpenCL kernel on the list
				size_t global_item_size = LIST_SIZE; // Process the entire lists
				size_t local_item_size = 64; // Divide work items into groups of 64
				ret = clEnqueueNDRangeKernel(command_queue, kernel, NULL, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
				//3 параметр - не более CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS можно поставить NULL для автоматического определения
				//5 парметр - должен быть в 1 .. 2^CL_DEVICE_ADDRESS_BITS - 1
				//6 параметр - кол-во work-items в work-group должно быть не больше CL_DEVICE_MAX_WORK_GROUP_SIZE

				// Read the memory buffer C on the device to the local variable C
				ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
				//printf("after copying\n");
				// Display the result to the screen
				//for (i = 0; i < LIST_SIZE; i++)
				//	printf("%d + %d = %d\n", A[i], B[i], C[i]);
				st2 = omp_get_wtime();
				GetLocalTime(&st);
				printf("Opencl compute finished: %d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
				printf("\033[22;32m%f seconds.\n\033[01;37m", st2- st1);
				// Clean up
				ret = clFlush(command_queue);
				ret = clFinish(command_queue);
				ret = clReleaseKernel(kernel);
				ret = clReleaseProgram(program);
				ret = clReleaseMemObject(a_mem_obj);
				ret = clReleaseMemObject(b_mem_obj);
				ret = clReleaseMemObject(c_mem_obj);
				ret = clReleaseCommandQueue(command_queue);
				ret = clReleaseContext(context);
				//system("pause");
			}
		}
		else { printf(" Error %i in clGetDeviceIDs call !!!\n\n", ret); }
	}

	free(A);
	free(B);
	free(C);
*/
	return 0;
	//
	//std::cout << "Exit\n"; 
	int ch;
	printf("\n\tPress any key to exit...\n");
	_getch();

}

