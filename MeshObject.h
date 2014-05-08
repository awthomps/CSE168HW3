////////////////////////////////////////
// MeshObject.h
////////////////////////////////////////

#ifndef CSE168_MESHOBJECT_H
#define CSE168_MESHOBJECT_H

#include "Object.h"
#include "Triangle.h"

////////////////////////////////////////////////////////////////////////////////

class MeshObject:public Object {
public:
	MeshObject();
	~MeshObject();

	bool Intersect(const Ray &ray,Intersection &hit);

	void MakeBox(float x,float y,float z,Material *mtl=0);
	bool LoadPLY(const char *filename, Material *mtl);
	void Smooth();
	Triangle** getTriangles();
	int getNumTriangles() {	return NumTriangles; }

private:
	int NumVertexes,NumTriangles;
	Vertex *Vertexes;
	Triangle *Triangles;
};

////////////////////////////////////////////////////////////////////////////////

#endif
