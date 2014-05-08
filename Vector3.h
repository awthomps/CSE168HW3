////////////////////////////////////////
// Vector3.h
////////////////////////////////////////

#ifndef CSE168_VECTOR3_H
#define CSE168_VECTOR3_H

#include "Core.h"

////////////////////////////////////////////////////////////////////////////////

class Vector3 {
public:
	Vector3() : x(0.0f),y(0.0f),z(0.0f)							{}
	Vector3(float x0,float y0,float z0)							{x=x0; y=y0; z=z0;}
	void Set(float x0,float y0,float z0)						{x=x0; y=y0; z=z0;}
	void Zero()													{x=y=z=0.0f;}

	// Math routines
	void Add(const Vector3 &a)									{x+=a.x; y+=a.y; z+=a.z;}
	void Add(const Vector3 &a,const Vector3 &b)					{x=a.x+b.x; y=a.y+b.y; z=a.z+b.z;}
	void AddScaled(const Vector3 &a,float s)					{x+=s*a.x; y+=s*a.y; z+=s*a.z;}
	void AddScaled(const Vector3 &a,const Vector3 &b,float s)	{x=a.x+s*b.x; y=a.y+s*b.y; z=a.z+s*b.z;}
	void Subtract(const Vector3 &a)								{x-=a.x; y-=a.y; z-=a.z;}
	void Subtract(const Vector3 &a,const Vector3 &b)			{x=a.x-b.x; y=a.y-b.y; z=a.z-b.z;}
	void Negate()												{x=-x; y=-y; z=-z;}
	void Negate(const Vector3 &a)								{x=-a.x; y=-a.y; z=-a.z;}
	void Scale(float s)											{x*=s; y*=s; z*=s;}
	void Scale(float s,const Vector3 &a)						{x=s*a.x; y=s*a.y; z=s*a.z;}
	float Dot(const Vector3 &a) const							{return x*a.x+y*a.y+z*a.z;}
	void Cross(const Vector3 &a,const Vector3 &b)				{x=a.y*b.z-a.z*b.y; y=a.z*b.x-a.x*b.z; z=a.x*b.y-a.y*b.x;}
	float Magnitude() const										{return sqrtf(x*x+y*y+z*z);}
	float Magnitude2() const									{return x*x+y*y+z*z;}
	void Normalize()											{Scale(1.0f/Magnitude());}

	float Distance(const Vector3 &a) const						{return sqrtf((x-a.x)*(x-a.x)+(y-a.y)*(y-a.y)+(z-a.z)*(z-a.z));}
	float Distance2(const Vector3 &a) const						{return (x-a.x)*(x-a.x)+(y-a.y)*(y-a.y)+(z-a.z)*(z-a.z);}
	void Lerp(float t,const Vector3 &a,const Vector3 &b)		{float s=1.0f-t; x=s*a.x+t*b.x; y=s*a.y+t*b.y; z=s*a.z+t*b.z;}

	// Operator overloads
	Vector3 operator+(const Vector3 &a) const					{return Vector3(x+a.x,y+a.y,z+a.z);}
	const Vector3 &operator+=(const Vector3 &a)					{x+=a.x; y+=a.y; z+=a.z; return *this;}
	Vector3 operator-(const Vector3 &a) const					{return Vector3(x-a.x,y-a.y,z-a.z);}
	const Vector3 &operator-=(const Vector3 &a)					{x-=a.x; y-=a.y; z-=a.z; return *this;}
	Vector3 operator-() const									{return Vector3(-x,-y,-z);}

	Vector3 operator*(float s) const							{return Vector3(s*x,s*y,s*z);}
	Vector3 operator*(const Vector3 &a) const					{return Vector3(x*a.x,y*a.y,z*a.z);}
	Vector3 operator*=(float s)									{x*=s; y*=s; z*=s; return *this;}
	const Vector3 &operator*=(const Vector3 &a)					{x*=a.x; y*=a.y; z*=a.z; return *this;}

	Vector3 operator/(float s) const							{float inv=1.0f/s; return Vector3(x*inv,y*inv,z*inv);}
	Vector3 operator/(const Vector3 &a) const					{return Vector3(x/a.x,y/a.y,z/a.z);}
	const Vector3 &operator/=(float s)							{float inv=1.0f/s; x*=inv; y*=inv; z*=inv; return *this;}
	const Vector3 &operator/=(const Vector3 &a)					{x/=a.x; y/=a.y; z/=a.z; return *this;}

	float &operator[](int i)									{return(((float*)this)[i]);}
	const float operator[](int i) const							{return(((float*)this)[i]);}

	// Misc functions
	void Print(char *name=0) const								{if(name) printf("%s=",name); printf("{%f,%f,%f}\n",x,y,z);}

	// Static vectors
	static Vector3 XAXIS,YAXIS,ZAXIS;
	static Vector3 ORIGIN;

public:
	float x,y,z;
};

inline Vector3 operator*(float s,const Vector3 &a)				{return Vector3(s*a.x,s*a.y,s*a.z);}

////////////////////////////////////////////////////////////////////////////////

#endif
