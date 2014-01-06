#include "scene.h"
#ifndef RAYTRACER_H
#define RAYTRACER_H


class RayTracer {
public:
	Ray RayThruPixel(const Camera& camera, float i, float j, int height, int width);

    Color GetColor(const Ray& ray, const Scene& scene, int depth, float i, float j);   
       
    bool GetIntersection(const Ray& ray, const Scene& scene, const Object* &hitObject, vec3* hitPoint);
                 
    Color CalculateLighting(const Light& light, const Object* hitObject, const Ray& ray, const vec3& hitPoint, const float* attenuation);
    
    Ray TransformRay(const Ray& ray, const Object* object);
    
    Ray GenerateReflectedRay(const Ray& ray, const vec3& hit, const vec3& unitNormal);
};
#endif // RAYTRACER_H