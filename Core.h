////////////////////////////////////////
// Core.h
////////////////////////////////////////

#ifndef CSE168_CORE_H
#define CSE168_CORE_H

////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PI	3.14159265f

inline int Min(int a,int b)					{return a<b ? a : b;}
inline int Min(int a, int b, int c)	{ return Min(Min(a, b), c); }
inline float Min(float a,float b)			{return a<b ? a : b;}
inline float Min(float a, float b, float c)	{ return Min(Min(a, b), c); }
inline int Max(int a,int b)					{return a>b ? a : b;}
inline int Max(int a, int b, int c)	{ return Max(Max(a, b), c); }
inline float Max(float a,float b)			{return a>b ? a : b;}
inline float Max(float a, float b, float c)	{ return Max(Max(a, b), c); }
inline int Clamp(int x,int a,int b)			{if(x<a) return a; if(x>b) return b; return x;}
inline float Clamp(float x,float a,float b)	{if(x<a) return a; if(x>b) return b; return x;}

////////////////////////////////////////////////////////////////////////////////

#endif
