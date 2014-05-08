////////////////////////////////////////
// Light.h
////////////////////////////////////////

#ifndef CSE168_LIGHT_H
#define CSE168_LIGHT_H

#include "Vector3.h"
#include "Color.h"

////////////////////////////////////////////////////////////////////////////////

class Light {
public:
	Light()									{Intensity=1.0; BaseColor=Color::WHITE;}
	void SetBaseColor(const Color &col)		{BaseColor=col;}
	void SetIntensity(float i)				{Intensity=i;}

	virtual float Illuminate(const Vector3 &pos, Color &col, Vector3 &toLight, Vector3 &ltPos)=0;

protected:
	float Intensity;
	Color BaseColor;		// Actual color is Intensity*BaseColor
};

////////////////////////////////////////////////////////////////////////////////

#endif
