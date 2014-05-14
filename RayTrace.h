#ifndef CSE168_RAYTRACE_H
#define CSE168_RAYTRACE_H



#include "Intersection.h"
#include "Ray.h"
#include "Scene.h"


#define MAX_DEPTH 10

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


					
					if (hit.Mtl != NULL) {
						hit.Mtl->ComputeReflectance(shade, -sentRay.Direction, toLight, hit);
					}
					//add this lighting to the pixel
					hit.Shade.Add(shade);
				}
			}

			if (depth == MAX_DEPTH) return true;

			//Compute shade due to reflections/refractions:
			if (hit.Mtl != NULL) {
				Ray reflectedRay;
				Intersection newHit;
				Color surfaceColor;
				float newIntensity;

				reflectedRay.Origin = hit.Position;
				hit.Mtl->GenerateSample(surfaceColor, reflectedRay.Direction, -sentRay.Direction, hit);
				
				TraceRay(reflectedRay, newHit, depth + 1);
				newHit.Shade.Multiply(surfaceColor);
				newHit.Shade.Scale(1 / PI);
				hit.Shade.Add(newHit.Shade);
			}
			else std::cout << "This object has no material!" << std::endl;

			return true;
		}
	}

private:
	Scene* Scn;

	//Stats:
	int primaryRays, secondaryRays, shadowRays;
};


#endif