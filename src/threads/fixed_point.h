#ifndef FIXED_POINT_H
#define FIXED_POINT_H
#include<ctype.h>
#include<stdio.h>
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
#endif
