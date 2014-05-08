#pragma once

#ifndef CSE168_POINTLIGHT_H
#define CSE168_POINTLIGHT_H

#include "Vector3.h"
#include "Color.h"
#include "Light.h"

class PointLight: public Light
{
public:
	PointLight();
	~PointLight();

	float Illuminate(const Vector3 &pos, Color &col, Vector3 &toLight, Vector3 &ItPos) {
		toLight = Position - pos;
		float bright = Intensity / toLight.Magnitude2(); //inverse square falloff
		toLight.Normalize();
		col = BaseColor;
		ItPos = Position;
		return bright;
	}

	void SetPosition(Vector3 pos) { Position = pos; }

private:
	Vector3 Position;
};


#endif