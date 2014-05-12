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
		col.Scale(DiffuseColor,1.0f/PI);
	}

	void GenerateSample() {

	}
	
	void SetDiffuseColor(Color color) { DiffuseColor = color; }
private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////

#endif
