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
			std::vector<ShadowRay> shadowRays(Scn->GetNumLights());
			for (unsigned int i = 0; i < Scn->GetNumLights(); ++i) {
				shadowRays[i].hasBeenSet = false;
			}
			unsigned int shadowIndex = 0;
			for (unsigned int i = 0; i < Scn->GetNumLights(); ++i) {
				//declare locals:
				Color lightColor;
				Vector3 toLight, lightPos, in, out;
				Intersection obstruction;

				//compute lighting with this light 
				float intensity = Scn->GetLight(i).Illuminate(hit.Position, lightColor, toLight, lightPos);

				if (intensity == 0 || hit.Normal.Opposing(toLight)) continue;

				shadowRays[shadowIndex].ray.Direction = toLight;
				shadowRays[shadowIndex].ray.Origin = hit.Position;
				shadowRays[shadowIndex].hasBeenSet = true;
				shadowRays[shadowIndex].lightPos = lightPos;
				shadowRays[shadowIndex].intensity = intensity;
				shadowRays[shadowIndex].lightColor = lightColor;
				++shadowIndex;
			}

			if (depth == MAX_DEPTH) {
				//delete shadowRays; // clean
				return true;
			}

			shadowIndex = 0;
			while (shadowIndex < shadowRays.size() && shadowRays[shadowIndex].hasBeenSet) {
				//check if light is blocked:

				bool hasObstruction = false;
				if (Scn->Intersect(shadowRays[shadowIndex].ray, shadowRays[shadowIndex].obstruction)) {
					//check if the light is obstructed:
					float obstructionDistance = (shadowRays[shadowIndex].obstruction.Position - hit.Position).Magnitude();
					float lightDistance = (shadowRays[shadowIndex].lightPos - hit.Position).Magnitude();
					if (obstructionDistance < lightDistance) {
						//obstruction blocking light so continue
						hasObstruction = true;
					}
				}

				if (!hasObstruction) {
					Color shade = shadowRays[shadowIndex].lightColor;
					float dotResult = (shadowRays[shadowIndex].ray.Direction).Dot(hit.Normal);
					shade.Scale(((dotResult < 0) ? 0 : dotResult) * shadowRays[shadowIndex].intensity);

					//add this lighting to the pixel
					hit.Shade.Add(shade);
				}
				++shadowIndex; //increment the shadow ray index
			}
			
			//clean up:
			//delete[] shadowRays;
			return true;
		}
	}

private:
	Scene* Scn;

	//Stats:
	int primaryRays, secondaryRays, shadowRays;
};


#endif