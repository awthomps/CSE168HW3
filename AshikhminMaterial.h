
#ifndef CSE168_ASHIKHMINMATERIAL_H
#define CSE168_ASHIKHMINMATERIAL_H

#include <random>
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
		diffuse = DiffuseColor;
		diffuse.Scale(ComputeDiffuseComponent(in, out, hit));
		Rho.Add(specular);
		Rho.Add(diffuse);

		color.Multiply(Rho); // may not want to scale

	}

	float ComputeSpecularComponent(const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		Vector3 h, projectionH;
		float top, hDotK;

		//calculate top of equation:
		top = sqrt((RoughnessU + 1) * (RoughnessV + 1));
		h = in + out;
		h.Normalize();

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

	void GenerateSample(Color &color, Vector3 &sample, const Vector3 &in, const Intersection &hit) {
		float theta, phi, epsilon1, epsilon2, epsilon3, exponent, r, s;

		epsilon1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		phi = atan(sqrt((RoughnessU + 1) / (RoughnessV + 1)) * tan(PI * epsilon1 / 2.0));

		epsilon2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		exponent = 1 / ((RoughnessU * cos(phi) * cos(phi)) + (RoughnessV * sin(phi) * sin(phi)) + 1);
		theta = acos(pow((1 - epsilon2), exponent));

		epsilon3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Vector3 h, sampleH = Vector3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
		sampleH.Normalize(); //just in case...
		if (epsilon3 < 0.5) h.x = h.x * -1.0;
		if ((0.0 <= epsilon3 && epsilon3 <= 0.25) || (0.5 <= epsilon3 && epsilon3 <= 0.75)) h.z = h.z * -1.0;

		//orient the h vector into the surface coordinate space:
		Matrix34 coordinateSpace;
		coordinateSpace.a = hit.TangentU;
		coordinateSpace.b = hit.Normal;
		coordinateSpace.c = hit.TangentV;
		coordinateSpace.d = Vector3(0.0f, 0.0f, 0.0f);
		coordinateSpace.Transform(sampleH, h); // transform the vector

		//create reflection vector
		sample = -in - (2 * hit.Normal * (-in).Dot(hit.Normal));

		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		s = r * (SpecularLevel + DiffuseLevel);

		if (r < SpecularLevel) color = SpecularColor;
		else {
			color = Vector3(0.0f, 0.0f, 0.0f);
			Vector3 inCrossN = in;
			inCrossN.Cross(in, hit.Normal);
			color = DiffuseColor; //.AddScaled(DiffuseColor, cos(asin(inCrossN.Magnitude() / (in.Magnitude() * hit.Normal.Magnitude()))));
		}
		//color.Scale(1 / PI);
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
