
#ifndef CSE168_ASHIKHMINMATERIAL_H
#define CSE168_ASHIKHMINMATERIAL_H


#include "Material.h"
class AshikhminMaterial :
	public Material
{
public:
	void ComputeReflectance(Color &color, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		
	}

	float ComputeSpecularComponent(Color &color, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		float top = sqrt((RoughnessU + 1) * (RoughnessV + 1));
		Vector3 h = in + out;
		h.Normalize();
		float theta = 30; //TODO: HOW DO YOU CALCULATE THETA
		top *= pow(hit.Normal.Dot(h), (RoughnessU * cos(theta) * cos(theta) + RoughnessV * sin(theta) * sin(theta)));
		
		float bottom = 8 * PI;
		bottom *= (h.Dot(in) * Max(hit.Normal.Dot(in), hit.Normal.Dot(out)));

	}

	float FresnelReflectance(const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		
	}

	void GenerateSample() {

	}

	void SetSpecularLevel(float level) { SpecularLevel = level; }
	void SetDiffuseLevel(float level) { DiffuseLevel = level; }
	void SetDiffuseColor(Color color) { DiffuseColor = color; }
	void SetSpecularColor(Color color) { SpecularColor = color; }
	void SetRoughness(float levelU, float levelV) { RoughnessU = levelU; RoughnessV = levelV; }
private:
	Color DiffuseColor, SpecularColor;
	float SpecularLevel, DiffuseLevel, RoughnessU, RoughnessV;
};

#endif
