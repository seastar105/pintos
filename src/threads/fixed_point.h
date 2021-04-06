#ifndef FIXED_POINT_H
#define FIXED_POINT_H
typedef int FP;
static const int f_const = 1<<14;

FP int_to_FP(int n);
FP trunc_FP(FP x);
FP round_FP(FP x);
FP FP_add(FP x, FP y);
FP FP_sub(FP x, FP y);
FP FP_add_int(FP x, int n);
FP FP_sub_int(FP x, int n);
FP FP_mult(FP x, FP y);
FP FP_mult_int(FP x, int n);
FP FP_div(FP x, FP y);
FP FP_div_int(FP x, int n);

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
	if(x >= 0) return (x+(f_const>>1))/f_const;
	else return (x-(f_const>>1))/f_const;
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
	return ((int64_t) x) * f_const / y;
}

FP FP_div_int(FP x, int n)
{
	return x / n;
}
#endif
