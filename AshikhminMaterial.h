
#ifndef CSE168_ASHIKHMINMATERIAL_H
#define CSE168_ASHIKHMINMATERIAL_H


#include "Material.h"
class AshikhminMaterial :
	public Material
{
public:
	void ComputeReflectance(Color &color, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		Color Rho, specular, diffuse;
		Rho = Color(0.0f, 0.0f, 0.0f);
		specular = SpecularColor;
		specular.Scale(ComputeSpecularComponent(in, out, hit));
		//specular.getInVector3().Print("Specular");
		diffuse = DiffuseColor;
		diffuse.Scale(ComputeDiffuseComponent(in, out, hit));
		//diffuse.getInVector3().Print("Diffuse");
		Rho.Add(specular);
		Rho.Add(diffuse);

		//Rho.getInVector3().Print("Before 1/PI scale");
		//color.getInVector3().Print("current color");
		color.Multiply(Rho); // may not want to scale
		//color.getInVector3().Print("After 1/PI Scale");
	}

	float ComputeSpecularComponent(const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		Vector3 h, projectionH;
		float top, phi, hDotK;

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
		float hu, hv, hn;
		hu = h.Dot(hit.TangentU);
		hv = h.Dot(hit.TangentV);
		hn = h.Dot(hit.Normal);
		float exponent = (RoughnessU * hu * hu) + (RoughnessV * hv * hv);
		exponent /= (1 - (hn * hn));
		top *= pow(hit.Normal.Dot(h), exponent);
		
		float bottom = 8.0 * PI;
		bottom *= (h.Dot(in) * Max(hit.Normal.Dot(in), hit.Normal.Dot(out)));

		return FresnelReflectance(in.Dot(h)) * top / bottom;
	}

	float FresnelReflectance(float inDotH) {
		return SpecularLevel + ((1.0 - SpecularLevel) * pow((inDotH), 5.0));
	}

	float ComputeDiffuseComponent(const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		float accumulation;
		accumulation = 28.0 * DiffuseLevel / (23.0 * PI);
		accumulation *= (1 - SpecularLevel);
		accumulation *= (1 - pow(1 - (hit.Normal.Dot(in) / 2.0), 5.0));
		accumulation *= (1 - pow(1 - (hit.Normal.Dot(out) / 2.0), 5.0));
		return accumulation;
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
