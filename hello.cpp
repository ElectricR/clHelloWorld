#include "opencl_utils.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>


int main() {
	std::vector<cl_device_id> devices(1);
	unsigned int n = 1024 * 1024 * 100;
	opencl_utils::query_opencl(devices.data());	
	cl_int cl_status;
	cl_context context = clCreateContext(nullptr, 1, devices.data(), nullptr, nullptr, &cl_status);

	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in creating context");
	}

	cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, &cl_status);
	
	std::vector<unsigned char> a(n, 1);
	std::vector<unsigned char> b(n, 2);
	std::vector<unsigned char> c(n, 0);

	cl_mem ab = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR, n, a.data(), &cl_status);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in creating buffer");
	}

	cl_mem bb = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR, n, b.data(), &cl_status);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in creating buffer");
	}

	cl_mem cb = clCreateBuffer(context, 0, n, nullptr, &cl_status);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in creating buffer");
	}
	
	std::ifstream file("hello.cl");
	std::string program_src = std::string(
			std::istreambuf_iterator<char>(file), 
			std::istreambuf_iterator<char>());

	const char *pr = program_src.c_str();
	cl_program program = clCreateProgramWithSource(context, 1, &pr, 0, &cl_status);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in creating program");
	}

	cl_status = clBuildProgram(program, 1, devices.data(), nullptr, nullptr, nullptr);
	if (cl_status != CL_SUCCESS) {
		std::cout << "Error in building program" << std::endl;;
	}

	size_t log_size;
	cl_status = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in getting program logs");
	}
	std::vector<char> log(log_size);
	cl_status = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log.data(), nullptr);
	
	std::cout << log.data() << std::endl;

	cl_kernel kernel = clCreateKernel(program, "sum", &cl_status);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in creating kernel");
	}

	cl_status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &ab); 
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in setting kernel arg");
	}

	cl_status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bb); 
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in setting kernel arg");
	}

	cl_status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &cb); 
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in setting kernel arg");
	}

	cl_status = clSetKernelArg(kernel, 3, sizeof(unsigned int), &n); 
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in setting kernel arg");
	}

	size_t work_size = n;
	cl_event event;
	cl_status = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &work_size, nullptr, 0, nullptr, &event);
	if (cl_status == CL_INVALID_KERNEL_ARGS) {
		throw std::runtime_error("Error in queueing range");
	}

	cl_status = clWaitForEvents(1, &event);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in waiting for events");
	}
	cl_status = clEnqueueReadBuffer(queue, cb, CL_TRUE, 0, sizeof(unsigned char) * n, c.data(), 0, nullptr, nullptr);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in reading buffer");
	}
	for (int i = 0; i != n; ++i) {
		//std::cout << static_cast<int>(c[i]);
	}
	std::cout << "Success!" << std::endl;
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseMemObject(ab);
	clReleaseMemObject(bb);
	clReleaseMemObject(cb);
	return 0;
}
