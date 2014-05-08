#include "InstanceObject.h"


InstanceObject::InstanceObject()
{
}

InstanceObject::InstanceObject(Object &obj) {
	SetChild(obj);
}


InstanceObject::~InstanceObject()
{
}

void InstanceObject::SetChild(Object &obj) { Child = &obj; }
void InstanceObject::SetMatrix(Matrix34 &mtx) {
	Matrix.Copy(mtx);
	Inverse.Copy(mtx);
	Inverse.Inverse();
}

bool InstanceObject::Intersect(const Ray &ray, Intersection &hit) {
	Ray ray2;
	Inverse.Transform(ray.Origin, ray2.Origin);
	Inverse.Transform3x3(ray.Direction, ray2.Direction);
	if(Child->Intersect(ray2, hit) == false) return false;
	Matrix.Transform(hit.Position, hit.Position);
	Matrix.Transform3x3(hit.Normal, hit.Normal);
	hit.HitDistance = ray.Origin.Distance(hit.Position); //correct for any scaling
	return true;
}