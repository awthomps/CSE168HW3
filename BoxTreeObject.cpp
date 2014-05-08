#include "BoxTreeObject.h"
#include <iostream>

BoxTreeObject::BoxTreeObject()
{
	RootNode = new BoxTreeNode();
}


BoxTreeObject::~BoxTreeObject()
{
}

void BoxTreeObject::Construct(MeshObject &mesh) {
	
	watch.StartTime("Start Construction.");
	Triangle** triangles = mesh.getTriangles();
	int numTriangles = mesh.getNumTriangles();
	RootNode->Contruct(numTriangles, triangles);
	watch.CheckTime("Construction Finished.");
}

bool BoxTreeObject::Intersect(const Ray &ray, Intersection &hit) {
	float t;
	bool success = false;
	if (RootNode->TestRay(ray, t)){// || RootNode->ContainsPoint(ray.Origin)){
		success = RootNode->Intersect(ray, hit);

		if (success) {
			std::cout << "";
		}
	}
	return success;
}