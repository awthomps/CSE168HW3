#include "BoxTreeNode.h"
#include <iostream>

BoxTreeNode::BoxTreeNode()
{
	numTriangles = 0;
}


BoxTreeNode::~BoxTreeNode()
{
}


bool BoxTreeNode::Intersect(const Ray &ray, Intersection &hit) {
	float t1, t2;
	bool hit1 = false, hit2 = false;
	
	if (isLeaf && (numTriangles <= MAXTRIANGLESPERBOX) && (numTriangles > 0)) {
		//std::cout << "Triangle Count: " << numTriangles << std::endl;
		bool success = false;
		for (int i = 0; i < numTriangles; ++i) {
			if (Tri[i]->Intersect(ray, hit)) {
				success = true;
			}
		}
		return success;
	}

	//make sure children are not null
	if (Child1 == 0 && Child2 == 0) {
		//std::cout << "One of these is null." << std::endl;
		return false;
	}
	//check to see if children will hit the node or check if the ray exists starting from within the node:
	if(Child1 != 0) {
		hit1 = Child1->TestRay(ray, t1);// || Child1->ContainsPoint(ray.Origin);
	}
	if (Child2 != 0) {
		hit2 = Child2->TestRay(ray, t2);// || Child2->ContainsPoint(ray.Origin);
	}



	if (!hit1 && !hit2) {
		return false;
	} 
	else if (hit1 && !hit2) {
		//hit1 but not hit2
		return Child1->Intersect(ray, hit);
	}
	else if (hit2 && !hit1) {
		//hit2 but not hit1
		return Child2->Intersect(ray, hit);
	}
	else { //hit1 and hit2
		//decide which one is closer, also handle case where hit triangle also exists in other space:
		Vector3 hitPoint1, hitPoint2;
		Ray initialHit;
		if (t1 < t2) {
			bool child1Intersects = Child1->Intersect(ray, hit);
			if (child1Intersects) {
				if (hit.HitDistance > t2){//Child2->ContainsPoint(hit.Position)) {
					//we know there is a hit, lets check the other data for a closer hit
					Child2->Intersect(ray, hit);
				}
				return true;
			}
			else return Child2->Intersect(ray, hit);
		}
		else { //t2 <= t1
			bool child2Intersects = Child2->Intersect(ray, hit);
			if (child2Intersects) {
				if (hit.HitDistance > t1) {//Child1->ContainsPoint(hit.Position)){
					//we know there is a hit, lets check the other data for a closer hit
					Child1->Intersect(ray, hit);
				}
				return true;
			}
			else return Child1->Intersect(ray, hit);
		}
		

	}
}


void BoxTreeNode::Contruct(int count, Triangle **tri) {
	//Compute BoxMin & BoxMax to fit around all tri's:
	//init min and max to first triangles values:
	BoxMin.x = BoxMax.x = tri[0]->GetVtx(0).Position[0];
	BoxMin.y = BoxMax.y = tri[0]->GetVtx(0).Position[1];
	BoxMin.z = BoxMax.z = tri[0]->GetVtx(0).Position[2];

	//iterate through triangles
	for (int i = 0; i < count; ++i) {
		//iterate through vertices
		for (int j = 0; j < 3; ++j) {
			//iterate through x, y, and z checking boxmin and boxmax
			for (int k = 0; k < 3; ++k) {
				//if (BoxMin[k] > tri[i]->GetVtx(j).Position[k]) BoxMin[k] = tri[i]->GetVtx(j).Position[k];
				//if (BoxMax[k] < tri[i]->GetVtx(j).Position[k]) BoxMax[k] = tri[i]->GetVtx(j).Position[k];
				BoxMin[k] = Min(BoxMin[k], tri[i]->GetVtx(j).Position[k]);
				BoxMax[k] = Max(BoxMax[k], tri[i]->GetVtx(j).Position[k]);
			}
		}
	}

	//check ifthis is a leaf node
	if (count <= MAXTRIANGLESPERBOX) {
		for (int i = 0; i < count; ++i) {
			Tri[i] = tri[i];
		}
		numTriangles = count;
		isLeaf = true;
		return;
	}
	else numTriangles = 0;

	//determine largest box dimension: x, y, or z
	int largestDim = 0; //default to x dimension;
	float largestMag = BoxMax.x - BoxMin.x;

	//check y and z with x;
	for (int i = 1; i < 3; ++i) {
		float currentMag = BoxMax[i] - BoxMin[i];
		if (largestMag < currentMag) {
			largestMag = currentMag;
			largestDim = i;
		}
	}
	
	//std::cout << "Largest Dimension is:  " << largestDim << std::endl;


	//compute splitting plane halfway along largest dimension:
	Vector3 planeNormal, planePosition;

	//get center of box for center of plane:
	for (int i = 0; i < 3; ++i) {
		planePosition[i] = (BoxMax[i] - ((BoxMax[i] - BoxMin[i])/2));
	}
	//planePosition.Print();

	//find normal:
	switch (largestDim) {
		//X is the largest dimension:
		case 0:
		{
				  planeNormal = Vector3(0.0f, 1.0f, 1.0f);
				  planeNormal.Normalize();
		}
		//Y is the largest dimension:
		case 1:
		{
				  planeNormal = Vector3(1.0f, 0.0f, 1.0f);
				  planeNormal.Normalize();
		}
			//Z is the largest dimension:
		case 2:
		{
				  planeNormal = Vector3(1.0f, 1.0f, 0.0f);
				  planeNormal.Normalize();
		}
	}
	
	//Allocate two new temporary arrays
	Triangle **tri1 = new Triangle*[count];
	Triangle **tri2 = new Triangle*[count];
	int count1 = 0, count2 = 0;

	//place triangles into group 1 or group 2;
	for (int i = 0; i < count; ++i) {

		//check where the center lies
		Vector3 centerOfTriangle = tri[i]->ComputeCenter();
		float value = tri[i]->ComputeCenter()[largestDim];
		if ( value < planePosition[largestDim]) {
			//less, so place in group 1
			tri1[count1] = tri[i];
			++count1;
		}
		else {
			//greater or equal so place in group 2
			tri2[count2] = tri[i];
			++count2;
		}
	}

	//check if either group is empty, if so move (at least) 1 triangle into that group
	if (count1 == 0) {
		for (int i = 0; (i < SHARENUMBER) && count2 >= SHARENUMBER; ++i) {
			tri1[i] = tri2[count2 - 1 - i];
			//compensate the counts:
			++count1;
			--count2;
		}
		
		//std::cout << "Count 1 was zero" << std::endl;
	}
	else if (count2 == 0) {
		for (int i = 0; (i < SHARENUMBER) && count1 >= SHARENUMBER; ++i)  {
			tri2[i] = tri1[count1 - 1 - i];
			//compensate the counts:
			++count2;
			--count1;
		}
		//std::cout << "Count 2 was zero" << std::endl;
	}

	//recursively build sub-trees
	Child1 = new BoxTreeNode;
	Child2 = new BoxTreeNode;
	Child1->Contruct(count1, tri1);
	delete[]tri1; // free tri1
	Child2->Contruct(count2, tri2);
	delete[]tri2; //free tri2

	//free up arrays
	//delete []tri1;
	//delete []tri2;
}

bool BoxTreeNode::TestRay(const Ray &ray, float &t) {

	//get test t values for x, y, z (0,1,2)
	float t1[3], t2[3], tmin, tmax;
	for (int i = 0; i < 3; ++i) {
		t1[i] = ((BoxMin[i] - ray.Origin[i]) / ray.Direction[i]);
		t2[i] = ((BoxMax[i] - ray.Origin[i]) / ray.Direction[i]);
	}
	tmin = Max(Min(t1[0], t2[0]), Min(t1[1], t2[1]), Min(t1[2], t2[2]));
	tmax = Min(Max(t1[0], t2[0]), Max(t1[1], t2[1]), Max(t1[2], t2[2]));

	if (tmin <= tmax) {
		t = tmin;
		return true;
	}
	/*
	else if (tmin < 0) {
		t = tmin;
		return true;
	}*/
	//tmax < 0
	return false;
}

bool BoxTreeNode::ContainsPoint(Vector3 point) {
	//check if point lies in the bounding box
	for (int i = 0; i < 3; ++i) {
		if (point[i] < BoxMin[i] || point[i] > BoxMax[i]) return false;
	}
	return true;
}