#ifndef CSE168_RAYTRACE_H
#define CSE168_RAYTRACE_H



#include "Intersection.h"
#include "Ray.h"
#include "Scene.h"


#define MAX_DEPTH 2

struct ShadowRay {
	Ray ray;
	Intersection obstruction;
	bool hasBeenSet;
	Vector3 lightPos;
	float intensity;
	Color lightColor;
};

class RayTrace {


public:
	RayTrace(){}
	~RayTrace(){};

	void SetScene(Scene& s) { Scn = &s; }
	bool TraceRay(const Ray &sentRay, Intersection &hit, int depth = 1) {
		if (!Scn->Intersect(sentRay, hit)) {
			//no hits so skycolor:
			hit.Shade = Scn->GetSkyColor();
			return false;
		}
		else {
			for (unsigned int i = 0; i < Scn->GetNumLights(); ++i) {
				//declare locals:
				Color lightColor, shade;
				Vector3 toLight, lightPos, in, out;
				Intersection obstruction;
				Ray toLightRay;

				//compute lighting with this light 
				float intensity = Scn->GetLight(i).Illuminate(hit.Position, lightColor, toLight, lightPos);

				if (intensity == 0 || hit.Normal.Opposing(toLight)) continue;

				//check if light is blocked:
				toLightRay.Direction = toLight;
				toLightRay.Origin = hit.Position;

				bool shadowRayBlocked = false;
				if (Scn->Intersect(toLightRay, obstruction)) {
					//check if the light is obstructed:
					float obstructionDistance = (obstruction.Position - hit.Position).Magnitude();
					float lightDistance = (lightPos - hit.Position).Magnitude();
					if (obstructionDistance < lightDistance) {
						//obstruction blocking light so continue
						shadowRayBlocked = true;
					}
				}

				if (!shadowRayBlocked) {
					Color shade = lightColor;
					float dotResult = (toLight).Dot(hit.Normal);
					shade.Scale(((dotResult < 0) ? 0 : dotResult) * intensity * PI);


					Ray reflectedRay;
					reflectedRay.Origin = hit.Position;
					if (hit.Mtl != NULL) {
						hit.Mtl->ComputeReflectance(shade, -sentRay.Direction, toLight, hit);
					}
					//add this lighting to the pixel
					hit.Shade.Add(shade);
				}
			}

			if (depth == MAX_DEPTH) return true;

			return true;
		}
	}

private:
	Scene* Scn;

	//Stats:
	int primaryRays, secondaryRays, shadowRays;
};


#endif