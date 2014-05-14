////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#ifndef CSE168_LAMBERTMATERIAL_H
#define CSE168_LAMBERTMATERIAL_H

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial:public Material {
public:
	void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		col.Multiply(Color(DiffuseColor.getInVector3() * 1.0f/PI));
	}

	void GenerateSample(Color &color, Vector3 &sample, const Vector3 &in, const Intersection &hit) {
		float s, t, u, v;
		Vector3 preTrans;
		s = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		u = 2.0f * PI * s;
		t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		v = sqrt(1.0 - t);

		preTrans.x = v * cos(u);
		preTrans.y = sqrt(t);
		preTrans.z = v * sin(u);

		//orient the h vector into the surface coordinate space:
		Matrix34 coordinateSpace;
		coordinateSpace.a = hit.TangentU;
		coordinateSpace.b = hit.Normal;
		coordinateSpace.c = hit.TangentV;
		coordinateSpace.d = Vector3(0.0f, 0.0f, 0.0f);
		coordinateSpace.Transform3x3(preTrans, sample);
		//color = DiffuseColor;
		Vector3 inCrossN = in;
		inCrossN.Cross(in, hit.Normal);
		color = DiffuseColor; //.AddScaled(DiffuseColor, cos(asin(inCrossN.Magnitude() / (in.Magnitude() * hit.Normal.Magnitude()))));
		//color.Scale(1 / PI);
	}
	
	void SetDiffuseColor(Color color) { DiffuseColor = color; }
private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////

#endif
