#include "opencl_utils.h"
#include <stdexcept>
#include <iostream>
#include <vector>

void opencl_utils::query_platform(cl_platform_id id) {
	std::vector<char> platform_info;
	size_t platform_info_size;
	for (cl_platform_info platform_info_name : {CL_PLATFORM_PROFILE, 
				CL_PLATFORM_VERSION, 
				CL_PLATFORM_NAME, 
				CL_PLATFORM_VENDOR,
				CL_PLATFORM_EXTENSIONS}) {
		cl_int cl_status = clGetPlatformInfo(id, platform_info_name, 
				0, nullptr, &platform_info_size);
		if (cl_status != CL_SUCCESS) {
			throw std::runtime_error("Error in getting platform property");
		}
		platform_info.resize(platform_info_size);
		cl_status = clGetPlatformInfo(id, platform_info_name, 
				platform_info_size, platform_info.data(), nullptr);
		std::cout << "\tPlatform property ";
		switch(platform_info_name) {
			case CL_PLATFORM_PROFILE: std::cout << "CL_PLATFORM_PROFILE"; break;
			case CL_PLATFORM_VERSION: std::cout << "CL_PLATFORM_VERSION"; break;
			case CL_PLATFORM_NAME: std::cout << "CL_PLATFORM_NAME"; break;
			case CL_PLATFORM_VENDOR: std::cout << "CL_PLATFORM_VENDOR"; break;
			case CL_PLATFORM_EXTENSIONS: std::cout << "CL_PLATFORM_EXTENSIONS"; break;
			default: break;
		}
		std::cout<< ":" << std::endl;
		std::cout << "\t\t" << platform_info.data() << std::endl;
	}
}

void opencl_utils::query_device(cl_device_id id, cl_device_id *requested_devices) {
	std::vector<char> device_info;
	size_t device_info_size;
	cl_int cl_status; 
	cl_status = clGetDeviceInfo(id, CL_DEVICE_NAME, 0, nullptr, &device_info_size);
	if (cl_status == CL_INVALID_DEVICE) {
		throw std::runtime_error("Error in getting device's info: Invalid device");
	}
	else if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in getting device's info");
	}
	device_info.reserve(device_info_size);
	cl_status = clGetDeviceInfo(id, CL_DEVICE_NAME, device_info_size, device_info.data(), nullptr);
	std::cout << "\t\tDevice name: " <<std::endl
	       << "\t\t\t" << device_info.data() << std::endl;

	cl_device_type device_type;
	cl_status = clGetDeviceInfo(id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, nullptr);
	std::cout << "\t\tDevice type:" <<std::endl
	       << "\t\t\t" << device_type << std::endl;

	cl_status = clGetDeviceInfo(id, CL_DEVICE_VERSION, 0, nullptr, &device_info_size);
	device_info.reserve(device_info_size);
	cl_status = clGetDeviceInfo(id, CL_DEVICE_VERSION, device_info_size, device_info.data(), nullptr);
	std::cout << "\t\tDevice version: " <<std::endl
	       << "\t\t\t" << device_info.data() << std::endl;

	cl_ulong mem_size;
	cl_status = clGetDeviceInfo(id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(mem_size), &mem_size, nullptr);
	std::cout << "\t\tDevice global memory size: " <<std::endl
	       << "\t\t\t" << mem_size << std::endl;

	if (device_type == CL_DEVICE_TYPE_GPU) {
		*requested_devices = id;
		std::cout <<"asdda";
	}

}

void opencl_utils::query_devices(cl_platform_id id, cl_device_id *requested_devices) {
	std::cout << std::endl;
	cl_int cl_status; 
	cl_uint device_count;
	cl_status = clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, 0, nullptr, &device_count);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in getting device IDs");
	}
	std::vector<cl_device_id> devices(device_count);
	cl_status = clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, device_count, devices.data(), nullptr);
	std::cout << "\tFound " << device_count << " device(s)" << std::endl;	

	for (cl_uint i = 0; i != device_count; ++i) {
		std::cout << "\tDevice №" << i << std::endl;
		query_device(devices[i], requested_devices);
	}
}


void opencl_utils::query_opencl(cl_device_id *devices) {
	cl_uint platform_count = 0;
	cl_int cl_status;
       	
	/* Get number of platforms */
	cl_status = clGetPlatformIDs(0, nullptr, &platform_count);

	std::vector<cl_platform_id> platforms(platform_count);

	/* Get platforms themselves */
	cl_status = clGetPlatformIDs(platform_count, platforms.data(), nullptr);
	if (cl_status != CL_SUCCESS) {
		throw std::runtime_error("Error in getting platforms info");
	}
	
	for (cl_uint i = 0; i != platform_count; ++i) {
		std::cout << "Platform №" << i << ":" << std::endl;
		query_platform(platforms[i]);
		query_devices(platforms[i], devices);
	}
}

