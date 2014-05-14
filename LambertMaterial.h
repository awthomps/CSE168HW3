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

	Vector3 GenerateSample(const Vector3 &in, const Intersection &hit) {

	}
	
	void SetDiffuseColor(Color color) { DiffuseColor = color; }
private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////

#endif
