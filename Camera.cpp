#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{ 
}


void Camera::SetFOV(float f) {
	VerticalFOV = PI * f / 180;
}
void Camera::SetAspect(float a) {
	Aspect = a;
}
void Camera::SetResolution(int x, int y) {
	XRes = x;
	YRes = y;
	BMP.Resize(x, y);
}

void Camera::LookAt(Vector3 &pos, Vector3 &target) {
	LookAt(pos, target, Vector3(0.0f, 1.0f, 0.0f));
}

void Camera::LookAt(Vector3 &pos, Vector3 &target, Vector3 &up) {
	WorldMatrix.d = pos;
	WorldMatrix.c = WorldMatrix.d - target;
	WorldMatrix.c.Normalize();
	WorldMatrix.a.Cross(up, WorldMatrix.c);
	WorldMatrix.a.Normalize();
	WorldMatrix.b.Cross(WorldMatrix.c, WorldMatrix.a);
}

void Camera::Render(Scene &s) {
	
	//start timer:
	watch.StartTime("Start Render.");

	//create the corners of the rendering window:

	//calculate modifiedB
	Vector3 modifiedB = WorldMatrix.b;
	modifiedB.Scale(tan(VerticalFOV / 2));
	Vector3 modifiedA = WorldMatrix.a;
	//calculate hfov and modifiedB
	float hfov = 2 * atan(Aspect * tan((VerticalFOV / 2)));
	modifiedA.Scale(tan(hfov / 2));

	topLeft = WorldMatrix.d - WorldMatrix.c + modifiedB - modifiedA;
	//topLeft.Print("upleft");
	topRight = WorldMatrix.d - WorldMatrix.c + modifiedB + modifiedA;
	//topRight.Print("upright");
	bottomLeft = WorldMatrix.d - WorldMatrix.c - modifiedB - modifiedA;
	//bottomLeft.Print("downleft");
	bottomRight = WorldMatrix.d - WorldMatrix.c - modifiedB + modifiedA;
	//bottomRight.Print("downright");
	right = (topRight - topLeft);
	rightDelta = right.Magnitude() / XRes;
	right.Normalize();
	up = (topLeft - bottomLeft);
	downDelta = up.Magnitude() / YRes;
	up.Normalize();

	int raycount = 0;
	for (int y = 0; y < YRes; ++y) {
		for (int x = 0; x < XRes; ++x) {
			RenderPixel(x, y, s);
			//if (raycount % 1000 == 0) std::cout << "Firing ray: " << raycount << std::endl;
			raycount++;
		}
	}
	watch.CheckTime("Render Finished.");
}
void Camera::SaveBitmap(char *filename) {
	BMP.SaveBMP(filename);
}

void Camera::RenderPixel(int x, int y, Scene &s) {
	Ray sentRay;
	sentRay.Origin = WorldMatrix.d;
	
	//compute ray direction:
	sentRay.Direction = bottomLeft;
	sentRay.Direction.AddScaled(right, (float(x) + 0.5f) * rightDelta);
	sentRay.Direction.AddScaled(up, (float(y) + 0.5f) * downDelta);

	sentRay.Direction = sentRay.Direction - WorldMatrix.d;
	sentRay.Direction.Normalize();

	Intersection hit;
	hit.HitDistance = 1000;
	hit.Shade = Color(0.0, 0.0, 0.0);
	rayTrace.SetScene(s);
	rayTrace.TraceRay(sentRay, hit, 1);

	BMP.SetPixel(x, y, hit.Shade.ToInt());
	
}
