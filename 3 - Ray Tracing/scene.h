#include <vector>
#include <stack>
#include <sstream>
#include "geometry.h"
using namespace std;

#ifndef SCENE_H
#define SCENE_H

void rightmultiply(const mat4 & M, stack<mat4> &transform_stack);

struct Camera {
	vec3 eye;
    vec3 center;
    vec3 up;
    float fovy;
    Camera(const vec3 &_eye, const vec3 &_center, const vec3 &_up, const float& _fovy) : eye(_eye), center(_center), up(_up), fovy(_fovy) {}
        
    // Default camera configuration
    Camera() : eye(0.0, 0.0, 5.0), center(0.0, 0.0, 0.0), up(0.0, 1.0, 0.0) {}
};
struct Light {
    vec3 position_direction;
    vec3 transform; // Lights transformed by modelview
    Color color; // Light Colors
         
    enum Type {directional, point};
    Type type;
    const vec3& position() const; // For point lights
    const vec3& direction() const; // For directional lights
};

class Scene
{
private:
	bool readvals (stringstream &s, const int numvals, float *values) ;

public:
	Scene();
    ~Scene();

    void readfile (const string &filename);
    string resultFile;
        
    Camera camera; 
        
    int maxDepth; // maximum depth of recursive ray tracing
    
    int width, height;

    vector<Light> lights;

    Materials materials; // Global material
    float attenuation[3]; 
    
    // For multiple objects 
    vector<Object*> objects;

	int maxVerts, maxVertNorms;

    vector<vec3> vertexBuffer; // vertex buffer
    vector<vec3> vertexBufferWithNormal, vertexNormalBuffer; // vertex normal, correspond to vertex with normal
};
#endif // SCENE_H