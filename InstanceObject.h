#pragma once

#ifndef CSE168_INSTANCEOBJECT_H
#define CSE168_INSTANCEOBJECT_H


#include "Object.h"
#include "Matrix34.h"

class InstanceObject :
	public Object
{
public:
	InstanceObject();
	InstanceObject(Object &obj);
	virtual ~InstanceObject();


	virtual bool Intersect(const Ray &ray, Intersection &hit);
	void SetChild(Object &obj);
	void SetMatrix(Matrix34 &mtx);

private:
	Matrix34 Matrix;
	Matrix34 Inverse;	//Pre-computed inverse of Matrix
	Object *Child;
};

#endif