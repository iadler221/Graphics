#include "raytracer.h"
#include "stdio.h"

Ray RayTracer::RayThruPixel(const Camera& camera, float i, float j, int height, int width) {

    vec3 w = glm::normalize(camera.eye - camera.center);
    vec3 u = glm::normalize(glm::cross(camera.up, w));
    vec3 v = glm::cross(w, u);
        
    float fovy = camera.fovy * PI / 180.0;
    float tan_x = tan(fovy / 2.0) * width / height;
    float beta = tan(fovy / 2.0) * (height/2.0 - i) / (height / 2.0);
    float alpha =  tan_x * (j - width/2.0) / (width / 2.0);

    return Ray(camera.eye, alpha*u + beta*v - w);

}

Ray RayTracer::TransformRay(const Ray& ray, const Object* object) {

    vec4 or_extend(ray.origin, 1.0); // vec4 because it turns into homogenous coordinates
    vec4 dir_extend(ray.direction, 0.0); // The homogenous coordinate w is 0

	// We calculate the inverse transform applied to the original ray
    or_extend = (or_extend * object->InversedTransform);
    dir_extend = (dir_extend * object->InversedTransform);

    // We come back to the actual coordinate system
	vec3 or = vec3(or_extend.x / or_extend.w, or_extend.y / or_extend.w, or_extend.z / or_extend.w);
    vec3 dir = vec3(dir_extend.x, dir_extend.y, dir_extend.z);
    return Ray(or, dir);

}

bool RayTracer::GetIntersection(const Ray& ray, const Scene& scene, const Object* &hitObject, vec3* hitPoint) {

    float mindtist = INF; // INFINITE
    hitObject = NULL;

    for (int i = 0; i < (int)scene.objects.size(); ++i) {
        // Ray in object space
        Ray transformedRay = TransformRay(ray, scene.objects[i]);
        float t; // Distance
        
		if (scene.objects[i]->Intersect(transformedRay, &t)) {
            // Get back the hit point
            vec3 hit_trans = transformedRay.origin + transformedRay.direction * t; // ray = origin + direction*distance
            
			// It turns into homogenous coordinates
			vec4 hit_extend(hit_trans, 1.0); 
            hit_extend = hit_extend * scene.objects[i]->transform;

			// We must come back to the actual coordinate system
            vec3 hit = vec3(hit_extend.x / hit_extend.w, hit_extend.y / hit_extend.w, hit_extend.z / hit_extend.w);
            
            t = glm::length(hit - ray.origin); // The norm determines the length of a vector
            if (t < mindtist) {
                mindtist = t;
                hitObject = scene.objects[i];
                *hitPoint = hit;
            }
        }
    }

    if (hitObject == NULL)
        return false;
    else {
        return true;
    }

}

Color RayTracer::GetColor(const Ray& ray, const Scene& scene, int depth, float pixH, float pixW) {

    if (depth > scene.maxDepth) {
        return BLACK;
    }

    const Object* hitObject;
    vec3 hitPoint;
    if (!GetIntersection(ray, scene, hitObject, &hitPoint)) {
        return BLACK;
	}
	else {
		Color color(hitObject->materials.ambient + hitObject->materials.emission);
		for (int i = 0; i < (int)scene.lights.size(); ++i) {

			if (scene.lights[i].type == Light::point) { // POINT LIGHT
				Ray shadowRay(scene.lights[i].position(), hitPoint - scene.lights[i].position());
            
				const Object* tmpObject;
				vec3 shadowHit;
            
				bool ok = GetIntersection(shadowRay, scene, tmpObject, &shadowHit);

				if (ok) {
					if (IsSameVector(hitPoint, shadowHit)) {
						color = color + CalculateLighting(scene.lights[i], hitObject, ray, hitPoint, scene.attenuation);
					}
				}

			} 
			else { // DIRECTIONAL LIGHT
				// This lonely line serves for all the scenes except scene6
				// color = color + CalculateLighting(scene.lights[i], hitObject, ray, hitPoint, scene.attenuation);

				// Everything that follows serves for all the scenes
				Ray shadowRay(hitPoint, hitPoint - scene.lights[i].direction());
            
				const Object* tmpObject;
				vec3 shadowHit;
            
				bool ok = GetIntersection(shadowRay, scene, tmpObject, &shadowHit);

				if (ok) {
					if (IsSameVector(hitPoint, shadowHit)) {
						color = color + CalculateLighting(scene.lights[i], hitObject, ray, hitPoint, scene.attenuation);
					}
					else { // For scene5 also changing the value of epsilon
						color = color + CalculateLighting(scene.lights[i], hitObject, ray, hitPoint, scene.attenuation);
					}
				}
				
			}
		}
    
		if (!hitObject->materials.specular.isZero()) {
			vec3 unitNormal = glm::normalize( hitObject->InterpolatePointNormal(hitPoint) );
			Ray reflectedRay = GenerateReflectedRay(ray, hitPoint, unitNormal);
        
			// Recursive call to trace the reflected ray
			Color tempColor = GetColor(reflectedRay, scene, depth+1, pixH, pixW); // depth+1 until reach the maximum
			color = color + hitObject->materials.specular * tempColor;
		}

		return color;
	}

}

Ray RayTracer::GenerateReflectedRay(const Ray& ray, const vec3& hit, const vec3& unitNormal) {
    vec3 p1 = ray.direction - (unitNormal * (2 * glm::dot(ray.direction, unitNormal)));
    return Ray(hit, p1);
}

Color RayTracer::CalculateLighting(const Light& light, const Object* hitObject, const Ray& ray, const vec3& hitPoint, const float* attenuation) {

    vec3 lightDirection;
    if (light.type == Light::point) { // POINT LIGHT
        lightDirection = glm::normalize(light.position() - hitPoint);
    } 
	else { // DIRECTIONAL LIGHT
        lightDirection = glm::normalize(light.direction());
	}
    
    vec3 normal = glm::normalize(hitObject->InterpolatePointNormal(hitPoint));
    
    const Materials& materials = hitObject->materials;
    float nDotL = max(glm::dot(normal, lightDirection), 0.0f);
    Color diffuse = materials.diffuse * light.color * nDotL;
    
    vec3 halfvec = glm::normalize(lightDirection + glm::normalize(-ray.direction));
    float nDotH = max(glm::dot(normal, halfvec), 0.0f);
    Color specular = materials.specular * light.color * pow(nDotH, materials.shininess);
        
    Color result = diffuse + specular;
    if (light.type == Light::point) { // POINT LIGHT
        float d = glm::length(light.position() - hitPoint);
        result = result * (1.0 / (attenuation[0] + attenuation[1] * d + attenuation[2] * pow(d, 2)));
    }

    return result;

}