#pragma once

#ifndef CSE168_BOXTREEOBJECT_H
#define CSE168_BOXTREEOBJECT_H

#include "MeshObject.h"
#include "Ray.h"
#include "Intersection.h"
#include "BoxTreeNode.h"
#include "StopWatch.h"



class BoxTreeObject : public Object
{
public:
	BoxTreeObject();
	~BoxTreeObject();
	void Construct(MeshObject &mesh);
	bool Intersect(const Ray &ray, Intersection &hit);

private:
	BoxTreeNode *RootNode;
	StopWatch watch;
};

#endif