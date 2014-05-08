////////////////////////////////////////
// MeshObject.cpp
////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "MeshObject.h"
#include "LambertMaterial.h"
#include "Matrix34.h"

////////////////////////////////////////////////////////////////////////////////

MeshObject::MeshObject() {
	NumVertexes=0;
	NumTriangles=0;

	Vertexes=0;
	Triangles=0;
}

////////////////////////////////////////////////////////////////////////////////

MeshObject::~MeshObject() {
	delete []Vertexes;
	delete []Triangles;
}

////////////////////////////////////////////////////////////////////////////////

bool MeshObject::Intersect(const Ray &ray, Intersection &hit) {
	bool success=false;
	for(int i=0;i<NumTriangles;i++)
		if(Triangles[i].Intersect(ray,hit)) success=true;
	return success;
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::MakeBox(float x,float y,float z,Material *mtl) {
	// Allocate arrays
	NumVertexes=24;
	NumTriangles=12;
	Vertexes=new Vertex[NumVertexes];
	Triangles=new Triangle[NumTriangles];
	if(mtl==0) mtl=new LambertMaterial;

	x*=0.5f;
	y*=0.5f;
	z*=0.5f;

	// Corners
	Vector3 p000(-x,-y,-z);
	Vector3 p001(-x,-y,z);
	Vector3 p010(-x,y,-z);
	Vector3 p011(-x,y,z);
	Vector3 p100(x,-y,-z);
	Vector3 p101(x,-y,z);
	Vector3 p110(x,y,-z);
	Vector3 p111(x,y,z);

	// Tex coords
	Vector3 t00(0.0f,0.0f,0.0f);
	Vector3 t01(0.0f,1.0f,0.0f);
	Vector3 t10(1.0f,0.0f,0.0f);
	Vector3 t11(1.0f,1.0f,0.0f);

	// Right
	Vertexes[0].Set(p101,Vector3::XAXIS,t00);
	Vertexes[1].Set(p100,Vector3::XAXIS,t10);
	Vertexes[2].Set(p110,Vector3::XAXIS,t11);
	Vertexes[3].Set(p111,Vector3::XAXIS,t01);
	Triangles[0].Init(&Vertexes[0],&Vertexes[1],&Vertexes[2],mtl);
	Triangles[1].Init(&Vertexes[0],&Vertexes[2],&Vertexes[3],mtl);

	// Left
	Vertexes[4].Set(p000,-Vector3::XAXIS,t00);
	Vertexes[5].Set(p001,-Vector3::XAXIS,t10);
	Vertexes[6].Set(p011,-Vector3::XAXIS,t11);
	Vertexes[7].Set(p010,-Vector3::XAXIS,t01);
	Triangles[2].Init(&Vertexes[4],&Vertexes[5],&Vertexes[6],mtl);
	Triangles[3].Init(&Vertexes[4],&Vertexes[6],&Vertexes[7],mtl);

	// Top
	Vertexes[8].Set(p011,Vector3::YAXIS,t00);
	Vertexes[9].Set(p111,Vector3::YAXIS,t10);
	Vertexes[10].Set(p110,Vector3::YAXIS,t11);
	Vertexes[11].Set(p010,Vector3::YAXIS,t01);
	Triangles[4].Init(&Vertexes[8],&Vertexes[9],&Vertexes[10],mtl);
	Triangles[5].Init(&Vertexes[8],&Vertexes[10],&Vertexes[11],mtl);

	// Bottom
	Vertexes[12].Set(p000,-Vector3::YAXIS,t00);
	Vertexes[13].Set(p100,-Vector3::YAXIS,t10);
	Vertexes[14].Set(p101,-Vector3::YAXIS,t11);
	Vertexes[15].Set(p001,-Vector3::YAXIS,t01);
	Triangles[6].Init(&Vertexes[12],&Vertexes[13],&Vertexes[14],mtl);
	Triangles[7].Init(&Vertexes[12],&Vertexes[14],&Vertexes[15],mtl);

	// Front
	Vertexes[16].Set(p001,Vector3::ZAXIS,t00);
	Vertexes[17].Set(p101,Vector3::ZAXIS,t10);
	Vertexes[18].Set(p111,Vector3::ZAXIS,t11);
	Vertexes[19].Set(p011,Vector3::ZAXIS,t01);
	Triangles[8].Init(&Vertexes[16],&Vertexes[17],&Vertexes[18],mtl);
	Triangles[9].Init(&Vertexes[16],&Vertexes[18],&Vertexes[19],mtl);

	// Back
	Vertexes[20].Set(p100,-Vector3::ZAXIS,t00);
	Vertexes[21].Set(p000,-Vector3::ZAXIS,t10);
	Vertexes[22].Set(p010,-Vector3::ZAXIS,t11);
	Vertexes[23].Set(p110,-Vector3::ZAXIS,t01);
	Triangles[10].Init(&Vertexes[20],&Vertexes[21],&Vertexes[22],mtl);
	Triangles[11].Init(&Vertexes[20],&Vertexes[22],&Vertexes[23],mtl);
}

////////////////////////////////////////////////////////////////////////////////



bool MeshObject::LoadPLY(const char *filename, Material *mtl) {
	// Open file
	FILE *f = fopen(filename, "r");
	if (f == 0) {
		printf("ERROR: MeshObject::LoadPLY()- Can't open '%s'\n", filename);
		return false;
	}

	// Read header
	char tmp[256];
	int numverts = 0, numtris = 0;
	int posprop = -99, normprop = -99;
	int props = 0;
	while (1) {
		fgets(tmp, 256, f);
		if (strncmp(tmp, "element vertex", 14) == 0)
			numverts = atoi(&tmp[14]);
		if (strncmp(tmp, "element face", 12) == 0)
			numtris = atoi(&tmp[12]);
		if (strncmp(tmp, "property", 8) == 0) {
			int len = strlen(tmp);
			if (strncmp(&tmp[len - 3], " x", 2) == 0) posprop = props;
			if (strncmp(&tmp[len - 3], "nx", 2) == 0) normprop = props;
			props++;
		}
		if (strcmp(tmp, "end_header\n") == 0) break;
	}
	if (posprop == -1) {
		printf("ERROR: MeshObject::LoadPLY()- No vertex positions found\n");
		fclose(f);
		return false;
	}

	// Read verts
	int i = 0;
	if (numverts>0) {
		NumVertexes = numverts;
		Vertexes = new Vertex[NumVertexes];

		for (i = 0; i<NumVertexes; i++) {
			fgets(tmp, 256, f);
			char *pch = strtok(tmp, " ");
			int prop = 0;
			while (pch) {
				if (prop == posprop) Vertexes[i].Position.x = float(atof(pch));
				if (prop == posprop + 1) Vertexes[i].Position.y = float(atof(pch));
				if (prop == posprop + 2) Vertexes[i].Position.z = float(atof(pch));
				if (prop == normprop) Vertexes[i].Normal.x = float(atof(pch));
				if (prop == normprop + 1) Vertexes[i].Normal.y = float(atof(pch));
				if (prop == normprop + 2) Vertexes[i].Normal.z = float(atof(pch));
				pch = strtok(0, " ");
				prop++;
			}
		}
	}

	// Read tris
	if (numtris>0) {
		if (mtl == 0) mtl = new LambertMaterial;
		NumTriangles = numtris;
		Triangles = new Triangle[numtris];
		for (i = 0; i<numtris; i++) {
			int count, i0, i1, i2;
			fscanf(f, "%d %d %d %d\n", &count, &i0, &i1, &i2);
			if (count != 3) {
				printf("ERROR: MeshObject::LoadPLY()- Only triangles are supported\n");
				fclose(f);
				return false;
			}
			Triangles[i].Init(&Vertexes[i0], &Vertexes[i1], &Vertexes[i2], mtl);
		}
	}

	// Smooth
	if (normprop<0) Smooth();

	// Close file
	fclose(f);
	printf("Loaded %d triangles from file '%s'\n", numtris, filename);
	return true;
}


void MeshObject::Smooth() {
	int i, j;
	for (i = 0; i<NumVertexes; i++)
		Vertexes[i].Normal.Zero();
	for (i = 0; i<NumTriangles; i++) {
		Triangle &tri = Triangles[i];
		Vector3 e1 = tri.GetVtx(1).Position - tri.GetVtx(0).Position;
		Vector3 e2 = tri.GetVtx(2).Position - tri.GetVtx(0).Position;
		Vector3 cross;
		cross.Cross(e1, e2);
		for (j = 0; j<3; j++)
			tri.GetVtx(j).Normal.Add(cross);
	}
	for (i = 0; i < NumVertexes; i++)
	{
		Vertexes[i].Normal.Normalize();
	}
}

Triangle** MeshObject::getTriangles() {
	Triangle** triangles;
	triangles = new Triangle*[NumTriangles];
	for (int i = 0; i < NumTriangles; i++) {
		triangles[i] = &Triangles[i];
	}
	return triangles;
}