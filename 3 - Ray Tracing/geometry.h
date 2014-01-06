#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>

#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI 3.14159265359
#define INF std::numeric_limits<float>::infinity(); // You can use 1e20 as well

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ; 
typedef glm::vec3 vec3 ; 
typedef glm::vec4 vec4 ; 
typedef unsigned char BYTE;

extern const float epsilon;
int sign(float x);

bool IsSameVector(const vec3& a, const vec3& b);

struct Ray {
    vec3 origin;
    vec3 direction;
    Ray(const vec3& _origin, const vec3& _direction);
};

struct Color
{
	float r, g, b;
    Color() : r(0.0), g(0.0), b(0.0) {}
    Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
    BYTE Rbyte() const { return std::min(r, 1.0f) * 255; }
    BYTE Gbyte() const { return std::min(g, 1.0f) * 255; }
    BYTE Bbyte() const { return std::min(b, 1.0f) * 255; }
    bool operator == (const Color& otherColor) const;
    Color operator * (const Color& otherColor) const;
    Color operator + (const Color& otherColor) const;
    Color operator * (const float scale) const;
    bool isZero() const;
};

const Color BLACK(0, 0, 0);
const Color WHITE(1.0, 1.0, 1.0);

// Objects have different colors
struct Materials {
	Color ambient;
	Color diffuse; 
    Color specular;
    Color emission; 
    float shininess;
    Materials();
};

class Object {
public:
	mat4 transform; 
    mat4 InversedTransform;
    Materials materials;        
    
    int index; // Identify the object for debugging
    
    enum shape {triangle, sphere} ;
    shape type; 
    
    Object();
    virtual ~Object();
    virtual bool Intersect(const Ray& ray, float* distance) const;
    virtual vec3 InterpolatePointNormal(const vec3& point) const;
};

class Sphere : public Object {
public:
	vec3 position;
    float radius;
    Sphere(const vec3& _o, const float& _r);
        
    virtual ~Sphere();
    virtual bool Intersect(const Ray& ray, float* distance) const;
    virtual vec3 InterpolatePointNormal(const vec3& point) const;
};

class Triangle : public Object {
public:
    vec3 vertexes[3], vertexNormals[3];
    vec3 &a, &b, &c; // vertices
    vec3 &na, &nb, &nc; // surface normal with each vertex
    
    Triangle(const vec3& _a, const vec3& _b, const vec3& _c, vec3 na = vec3(0,0,0), vec3 nb = vec3(0,0,0), vec3 nc = vec3(0,0,0));
    
    virtual ~Triangle();
    virtual bool Intersect(const Ray& ray, float* distance) const;
    virtual vec3 InterpolatePointNormal(const vec3& point) const;
};
#endif // GEOMETRY_H