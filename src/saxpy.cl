void __kernel saxpy(const unsigned int n,
                    const double a,
                    __global double *x,
                    __global double *y) {
	unsigned int i = get_global_id(0);
	y[i] = a*x[i] + y[i];
}
