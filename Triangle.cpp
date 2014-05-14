

#include "Triangle.h"
#include <iostream>


Triangle::Triangle() {};

bool Triangle::Intersect(const Ray &ray, Intersection &hit) const {
	Matrix34 M = Matrix34();
	Vector3 triA, triB, triC;
	triA = Vtx[0]->Position;
	triB = Vtx[1]->Position;
	triC = Vtx[2]->Position;
	//triA.Print("A");
	//triB.Print("B");
	//triC.Print("C");
	M.a = -ray.Direction;
	M.b = triB - triA;
	M.c = triC - triA;
	

	Vector3 bMa, cMa, crossResult, pMa;
	bMa = triB - triA;
	cMa = triC - triA;
	pMa = ray.Origin - triA;

	//det(M) = -d dot ((b-a) cross (c-a))
	//crossResult = bMa;
	crossResult.Cross(bMa, cMa);
	float DetM = (-ray.Direction).Dot(crossResult);
	//printf("DetM = %d\n", DetM);

	//check if ray is parallel to the plane:
	if (DetM == 0) return false;

	//alpha = -d dot ((p - a) cross (c - a)) /det(M)
	//crossResult = pMa;
	crossResult.Cross(pMa, cMa);
	float alpha = (-ray.Direction).Dot(crossResult) / DetM;
	//printf("alpha = %d\n", alpha);
	//check if alpha is in range
	if (alpha < 0 || alpha > 1) return false;

	//beta = -d dot (( b - a) cross (p - a)) / det(M)
	//crossResult = bMa;
	crossResult.Cross(bMa, pMa);
	float beta = (-ray.Direction).Dot(crossResult) / DetM;

	//check if beta is in range and if alpha+beta is in range
	if (beta < 0 || beta > 1 || (alpha+beta > 1)) return false;

	// t = (p - a) dot ((b - a) cross (c - a)) / det(M)
	//crossResult = bMa;
	crossResult.Cross(bMa, cMa);
	float t = pMa.Dot(crossResult) / DetM;

	//make sure that t is positive and it is less than hitdistance:
	//printf("t = %d\n", t);
	if ((t < SMALL_NUMBER) || (t >= hit.HitDistance)) return false;

	hit.HitDistance = t;
	hit.Mtl = Mtl;
	hit.Position = ray.Origin;
	hit.Position.AddScaled(ray.Direction, t);
	
	//normal:
	Vector3 nA = Vtx[0]->Normal;
	nA.Scale(1 - alpha - beta);
	Vector3 nB = Vtx[1]->Normal;
	nB.Scale(alpha);
	Vector3 nC = Vtx[2]->Normal;
	nC.Scale(beta);
	hit.Normal = nA + nB + nC;
	//hit.Normal.Print("Before normalization");
	//std::cout << "hit" << std::endl;
	hit.Normal.Normalize();
	//hit.Normal.Print("After normalization");

	//tangent:
	hit.TangentU.Cross(Vector3::YAXIS, hit.Normal);
	if (hit.TangentU.x == 0.0f, hit.TangentU.y == 0.0f, hit.TangentU.z == 0.0f) hit.TangentU.Cross(Vector3::ZAXIS, hit.Normal);
	hit.TangentU.Normalize();
	hit.TangentV.Cross(hit.Normal, hit.TangentU);

	//material
	if (Mtl != NULL) hit.Mtl = Mtl;

	return true;
}

Vector3 Triangle::ComputeCenter() {
	Vector3 a, b, c, bMa, cMa, center;
	float alpha = 0.5, beta = 0.5;
	a = Vtx[0]->Position;
	b = Vtx[1]->Position;
	c = Vtx[2]->Position;


	//q = a + alpha(b - a) + beta(c-a);
	bMa = b - a;
	bMa.Scale(alpha);
	cMa = c - a;
	cMa.Scale(beta);
	center = a + bMa + cMa;
	return center;
}