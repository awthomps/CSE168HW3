
#ifndef CSE168_ASHIKHMINMATERIAL_H
#define CSE168_ASHIKHMINMATERIAL_H


#include "Material.h"
class AshikhminMaterial :
	public Material
{
public:
	void ComputeReflectance(Color &color, const Vector3 &in, const Vector3 &out, const Intersection &hit) {

	}

	void SetSpecularLevel(float level) {

	}

	void SetDiffuseLevel(float level) {

	}

	void SetDiffuseColor(Color color) {

	}

	void SetSpecularColor(Color color) {

	}

	void SetRoughness(float levelA, float levelB) {

	}
};

#endif
