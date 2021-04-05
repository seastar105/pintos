#include "fixed_point.h"

FP int_to_FP(int n)
{
	return n * f_const;
}

FP trunc_FP(FP x)
{
	return x / f_const;
}

FP round_FP(FP x)
{
	if(x >= 0) return (x+(f>>1))/f;
	else return (x-(f>>1))/f;
}

FP FP_add(FP x, FP y)
{
	return x + y;
}

FP FP_sub(FP x, FP y)
{
	return x - y;
}

FP FP_add_int(FP x, int n)
{
	return x + n * f_const;
}

FP FP_sub_int(FP x, int n)
{
	return x - n * f_const;
}

FP FP_mult(FP x, FP y)
{
	return ((int64_t) x) * y / f_const;
}

FP FP_mult_int(FP x, int n)
{
	return x * n;
}

FP FP_div(FP x, FP y)
{
	return ((int64_t) x) * f / y;
}

FP FP_div_int(FP x, int n)
{
	return x / n;
}
