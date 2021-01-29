



__kernel void sum(__global const unsigned char *a,
			   __global const unsigned char *b,
			   __global unsigned char *c,
			   unsigned int n) {
	int id = get_global_id(0);
	
	if (id > n) return;
	
	c[id] = a[id] + b[id];
}


