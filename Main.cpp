////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "MeshObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "BoxTreeObject.h"
#include "AshikhminMaterial.h"
#include "LambertMaterial.h"

void project1();
void project2();
void project3();

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
	project3();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void project1() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Create boxes
	MeshObject box1;
	box1.MakeBox(5.0f,0.1f,5.0f);
	scn.AddObject(box1);

	MeshObject box2;
	box2.MakeBox(1.0f,1.0f,1.0f);

	InstanceObject inst1(box2);
	Matrix34 mtx;
	mtx.MakeRotateX(0.5f);
	mtx.d.y=1.0f;
	inst1.SetMatrix(mtx);
	scn.AddObject(inst1);

	InstanceObject inst2(box2);
	mtx.MakeRotateY(1.0f);
	mtx.d.Set(-1.0f,0.0f,1.0f);
	inst2.SetMatrix(mtx);
	scn.AddObject(inst2);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(0.5f);
	sunlgt.SetDirection(Vector3(-0.5f, -1.0f, -0.5f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(2.0f);
	redlgt.SetPosition(Vector3(2.0f, 2.0f, 0.0f));
	scn.AddLight(redlgt);

	// Create camera
	Camera cam;
	cam.LookAt(Vector3(2.0f, 2.0f, 5.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	cam.SetResolution(800, 600);
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);

	// Render image
	cam.Render(scn);
	cam.SaveBitmap("project1.bmp");
}

////////////////////////////////////////////////////////////////////////////////


void project2() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));

	// Create ground
	MeshObject ground;
	ground.MakeBox(5.0f, 0.1f, 5.0f);
	scn.AddObject(ground);

	// Create dragon
	MeshObject dragon;
	dragon.LoadPLY("dragon.ply", 0);
	dragon.Smooth();

	BoxTreeObject tree;
	tree.Construct(dragon);
	scn.AddObject(tree);

	// Create instance
	InstanceObject inst(tree);
	Matrix34 mtx;
	mtx.MakeRotateY(PI);
	mtx.d.Set(-0.05f, 0.0f, -0.1f);
	inst.SetMatrix(mtx);
	scn.AddObject(inst);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(Vector3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(0.02f);
	redlgt.SetPosition(Vector3(-0.2f, 0.2f, 0.2f));
	scn.AddLight(redlgt);

	PointLight bluelgt;
	bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
	bluelgt.SetIntensity(0.02f);
	bluelgt.SetPosition(Vector3(0.1f, 0.1f, 0.3f));
	scn.AddLight(bluelgt);

	// Create camera
	Camera cam;
	cam.LookAt(Vector3(-0.1f, 0.1f, 0.2f), Vector3(-0.05f, 0.12f, 0.0f));
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);
	cam.SetResolution(800, 600);

	// Render image
	cam.Render(scn);
	cam.SaveBitmap("project2.bmp");
}
void project3() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Materials
	const int nummtls = 4;
	AshikhminMaterial mtl[nummtls];

	// Diffuse
	mtl[0].SetSpecularLevel(0.0f);
	mtl[0].SetDiffuseLevel(1.0f);
	mtl[0].SetDiffuseColor(Color(0.7f, 0.7f, 0.7f));

	// Roughened copper
	mtl[1].SetDiffuseLevel(0.0f);
	mtl[1].SetSpecularLevel(1.0f);
	mtl[1].SetSpecularColor(Color(0.9f, 0.6f, 0.5f));
	mtl[1].SetRoughness(100.0f, 100.0f);

	// Anisotropic gold
	mtl[2].SetDiffuseLevel(0.0f);
	mtl[2].SetSpecularLevel(1.0f);
	mtl[2].SetSpecularColor(Color(0.95f, 0.7f, 0.3f));
	mtl[2].SetRoughness(1.0f, 1000.0f);

	// Red plastic
	mtl[3].SetDiffuseColor(Color(1.0f, 0.1f, 0.1f));
	mtl[3].SetDiffuseLevel(0.8f);
	mtl[3].SetSpecularLevel(0.2f);
	mtl[2].SetSpecularColor(Color(1.0f, 1.0f, 1.0f));
	mtl[3].SetRoughness(1000.0f, 1000.0f);

	// Load dragon mesh
	MeshObject dragon;
	dragon.LoadPLY("dragon.ply", 0);

	// Create box tree 
	BoxTreeObject tree;
	tree.Construct(dragon);

	// Create dragon instances
	Matrix34 mtx;
	for (int i = 0; i<nummtls; i++) {
		InstanceObject *inst = new InstanceObject(tree);
		mtx.d.Set(0.0f, 0.0f, -0.1f*float(i));
		inst->SetMatrix(mtx);
		inst->SetMaterial(&mtl[i]);
		scn.AddObject(*inst);
	}

	// Create ground
	LambertMaterial lambert;
	lambert.SetDiffuseColor(Color(0.3f, 0.3f, 0.35f));

	MeshObject ground;
	ground.MakeBox(2.0f, 0.11f, 2.0f, &lambert);
	scn.AddObject(ground);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(Vector3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);

	// Create camera
	Camera cam;
	cam.LookAt(Vector3(-0.5f, 0.25f, -0.2f), Vector3(0.0f, 0.15f, -0.15f));
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);
	cam.SetResolution(800, 600);
	cam.SetSuperSample(10);

	// Render image
	cam.Render(scn);
	cam.SaveBitmap("project3.bmp");
}