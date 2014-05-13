
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
		Vector3 h, projectionH;
		float top, phi;

		//calculate top of equation:
		top = sqrt((RoughnessU + 1) * (RoughnessV + 1));
		h = in + out;
		h.Normalize();

		//find projection of h on plane
		projectionH.Cross(h, hit.Normal);
		projectionH.Cross(hit.Normal, projectionH); //find projection
		projectionH.Normalize();

		//find angle between projectionH and TangentU vector:
		phi = acos(hit.TangentU.Dot(projectionH));
		top *= pow(hit.Normal.Dot(h), (RoughnessU * cos(phi) * cos(phi) + RoughnessV * sin(phi) * sin(phi)));
		
		float bottom = 8 * PI;
		bottom *= (h.Dot(in) * Max(hit.Normal.Dot(in), hit.Normal.Dot(out)));

	}

	Color FresnelReflectance(float inCrossH) {
		//Color InverseSpecularColor = Color()
		//return SpecularColor + ((1 - SpecularColor) )
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
