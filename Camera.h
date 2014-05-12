#pragma once


#ifndef CSE168_CAMERA_H
#define CSE168_CAMERA_H

#include "Matrix34.h"
#include "Vector3.h"
#include "Bitmap.h"
#include "Scene.h"
#include "Material.h"
#include "StopWatch.h"
#include "RayTrace.h"
#include <iostream>

class Camera
{
public:
	Camera();
	~Camera();

	void SetFOV(float f);
	void SetAspect(float a);
	void SetResolution(int x, int y);
	void LookAt(Vector3 &pos, Vector3 &target, Vector3 &up);
	void LookAt(Vector3 &pos, Vector3 &target);

	void Render(Scene &s);
	void SaveBitmap(char *filename);
	void RenderPixel(int x, int y, Scene &s);
	void SetSuperSample(int superSampleVal) { superSample = superSampleVal; }


private:

	int XRes, YRes, superSample;
	Matrix34 WorldMatrix;
	float VerticalFOV;
	float Aspect;
	Bitmap BMP;
	Vector3 topLeft, topRight, bottomLeft, bottomRight, right, up;
	float rightDelta;
	float downDelta;
	StopWatch watch;
	RayTrace rayTrace;
};

#endif

