#pragma once

#include <CL/cl.h>

namespace opencl_utils {

void query_platform(cl_platform_id id);
void query_device(cl_device_id id, cl_device_id *);
void query_devices(cl_platform_id id, cl_device_id *);
void query_opencl(cl_device_id *);

}
