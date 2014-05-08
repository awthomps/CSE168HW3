#pragma once

#ifndef CSE168_DIRECTLIGHT_H
#define CSE168_DIRECTLIGHT_H

#include "Light.h"
#include "Vector3.h"
class DirectLight :
	public Light
{
public:
	DirectLight();
	~DirectLight();

	float Illuminate(const Vector3 &pos, Color &col, Vector3 &toLight, Vector3 &ItPos) {

		toLight = -Direction;
		col = BaseColor;
		ItPos = pos - (1000000.0 * Direction); // Create virtual position)
		return Intensity;
	}

	void SetDirection(Vector3 dir) { Direction = dir; Direction.Normalize(); }

private:
	Vector3 Direction;
};

#endif 
