#pragma once

#ifndef CSE168_BOXTREENODE_H
#define CSE168_BOXTREENODE_H

#define MAXTRIANGLESPERBOX 10
#define SHARENUMBER 1

#include "Vector3.h"
#include "Intersection.h"
#include "Ray.h"
#include "Triangle.h"
class BoxTreeNode
{
public:
	BoxTreeNode();
	~BoxTreeNode();


	bool Intersect(const Ray &ray, Intersection &hit);
	void Contruct(int count, Triangle **tri);
	bool TestRay(const Ray &ray, float &t);
	bool ContainsPoint(Vector3 point);

private:
	Vector3 BoxMin, BoxMax;
	BoxTreeNode *Child1, *Child2;
	Triangle *Tri[MAXTRIANGLESPERBOX];
	int numTriangles;
	bool isLeaf = false;
	
};

#endif

