// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
	float radians = degrees*pi/180.f;
	mat3 I(1.0);
	mat3 m1 = mat3(pow(axis.x, 2), axis.x*axis.y, axis.x*axis.z, axis.x*axis.y, pow(axis.y, 2), axis.y*axis.z, axis.x*axis.z, axis.y*axis.z, pow(axis.z, 2));
	mat3 m2 = mat3(0, axis.z, -axis.y, -axis.z, 0, axis.x, axis.y, -axis.x, 0);
	mat3 rotateMatrix = cos(radians)*I+(1-cos(radians))*m1+sin(radians)*m2;

  // You will change this return call
    return rotateMatrix;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
	mat3 rotateMatrix = rotate(degrees, up);
	eye = rotateMatrix*eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
	mat3 mDual = mat3(0, -eye.z, eye.y, eye.z, 0, -eye.x, -eye.y, eye.x, 0);
	vec3 u = glm::normalize(mDual*up);
	mat3 rotateMatrix = rotate(degrees, u);
	eye = eye*rotateMatrix;
	up = up*rotateMatrix;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
	vec3 center = vec3(0,0,0); 
	vec3 a = eye - center; 
	vec3 b = up;
	vec3 w = glm::normalize(a); 
	vec3 u = glm::normalize(glm::cross(b,w)); 
	vec3 v = glm::cross(w,u);
	
	mat4 matrix = mat4(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0,glm::dot(u,-eye),glm::dot(v,-eye),glm::dot(w,-eye),1);

  // You will change this return call
    return matrix;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
