#include "geometry.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>

const float epsilon = 1e-6; // 1e-6 means 0.000001 For scene5 1e-3 would be better
int sign(float x) { return x > epsilon? 1 : x < -epsilon? -1 : 0; }

bool IsSameVector(const vec3& a, const vec3& b) {
    return glm::dot(b-a, b-a) < epsilon;
}

bool Color::operator == (const Color &otherColor) const {
    return r == otherColor.r && g == otherColor.g && b == otherColor.b;
}
Color Color::operator * (const Color& otherColor) const {
    return Color(r * otherColor.r, g * otherColor.g, b * otherColor.b);
}
Color Color::operator + (const Color& otherColor) const {
    return Color(r + otherColor.r, g + otherColor.g, b + otherColor.b);
}
Color Color::operator * (const float scale) const {
    return Color(r * scale, g * scale, b * scale);
}
bool Color::isZero() const { return Rbyte() == 0 && Gbyte() == 0 && Bbyte() == 0; }

Materials::Materials() : shininess(0.0) {}

Ray::Ray(const vec3& _origin, const vec3& _direction) : origin(_origin), direction(_direction) {}

Object::Object() : transform(1.0), InversedTransform(1.0) {}

bool Object::Intersect(const Ray& ray, float* distance) const {
    std::cerr << "Ray should not intersect with abstract object" << std::endl;
    throw 2; // This should never happen
}

vec3 Object::InterpolatePointNormal(const vec3& point) const {
    std::cerr << "Can't interpolate normal in abstract object" << std::endl;
    throw 2;
}

Sphere::Sphere(const vec3& _position, const float& _radius) : position(_position), radius(_radius) {
    type = sphere;
}

bool Sphere::Intersect(const Ray& ray, float* distance) const {

	const vec3& or = ray.origin;
    const vec3& dir = ray.direction;
    
	// ax^2 + bx + c = 0
    float a = glm::dot(dir, dir);
    float b = 2 * glm::dot(dir, or-position);
    float c = glm::dot(or-position, or-position) - pow(radius, 2);

	//b^2 - 4*a*c
    float discriminant = pow(b, 2) - 4*a*c;

    if (discriminant < -epsilon) {
        return false;
    }

    discriminant = fabs(discriminant); // fabs(x) returns the absolute value of x: |x|   
    // closest intersection point
    float t = std::min((-b - sqrt(discriminant)) / (2*a), (-b + sqrt(discriminant)) / (2*a));
    
    if (t < 1e-2) { // 1e-2 means 0.01
        return false;
    } else {
        *distance = t;
        return true;
    }
}
vec3 vec3TimeMat4(const vec3& a, const mat4& mat) {
    vec4 a_extend = vec4(a, 1.0f) * mat;
    return vec3(a_extend.x / a_extend.w, a_extend.y / a_extend.w, a_extend.z / a_extend.w);
}
vec3 ray3TimeMat4(const vec3& a, const mat4& mat) {
    return vec3(vec4(a, 0.0f) * mat);
}
vec3 Sphere::InterpolatePointNormal(const vec3& point) const {
    vec3 p = vec3TimeMat4(point, this->InversedTransform);
    return vec3(vec4(p-position, 0.0f) * glm::transpose(this->InversedTransform));
}

Triangle::Triangle(const vec3& _a, const vec3& _b, const vec3& _c, vec3 _na, vec3 _nb, vec3 _nc) : 
    a(vertexes[0]),
    b(vertexes[1]),
    c(vertexes[2]),
    na(vertexNormals[0]),
    nb(vertexNormals[1]),
    nc(vertexNormals[2]) {
        a = _a;
        b = _b;
        c = _c;
        type = triangle;
        // no specified normal
        if (_na == vec3(0,0,0)) {
            na = nb = nc = glm::cross(b-a, c-a);
        } 
		else {
            na = _na;
            nb = _nb;
            nc = _nc;
        }
    };

bool Triangle::Intersect(const Ray& ray, float* distance) const {

    vec3 n = glm::cross(b-a, c-a); // n is the normal
    const vec3& or = ray.origin;
    const vec3& dir = ray.direction;
    
    // ray parallel with plane
    if (sign(glm::dot(dir, n)) == 0) {
        return false; // There is no intersection
    }
    
    // plane ray intersection -- t = (A·n - or·n) / (dir·n)
    float t = (glm::dot(a,n) - glm::dot(or,n)) / glm::dot(dir,n);
    vec3 p0 = or + (dir * t);
    
    if (t < 1e-2) { // 1e-2 means 0.01
        return false;
    }
    
    vec3 tmp_nb = glm::cross(c-p0, a-p0);
    vec3 tmp_nc = glm::cross(a-p0, b-p0);
    
    float beta = glm::dot(n, tmp_nb) / glm::dot(n, n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n, n);
    float alpha = 1.0 - beta - gamma;

    if (-epsilon < beta && beta < 1.0+epsilon && -epsilon < gamma && gamma < 1.0+epsilon && -epsilon < alpha && alpha < 1.0+epsilon) { // The point is inside the triangle
        *distance = t;
        return true; 
    } 
	else { // The point is outside the triangle
        return false;
	}
}
vec3 Triangle::InterpolatePointNormal(const vec3& point) const {
    vec3 p = vec3TimeMat4(point, this->InversedTransform);
    vec3 n = glm::cross(b-a, c-a);
    vec3 tmp_nb = glm::cross(c-p, a-p);
    vec3 tmp_nc = glm::cross(a-p, b-p);
    
    float beta = glm::dot(n, tmp_nb) / glm::dot(n,n);
    float gamma = glm::dot(n, tmp_nc) / glm::dot(n,n);
    float alpha = 1.0 - beta - gamma;
    vec3 ret = (na * alpha) + (nb * beta) + (nc * gamma);
    return vec3(vec4(ret, 0.0f) * glm::transpose(this->InversedTransform));
}

Object::~Object() {
}
Triangle::~Triangle() {
}
Sphere::~Sphere() {
}