// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
	float radians = degrees*pi/180.f;
	mat3 I(1.0);
	vec3 axisN = glm::normalize(axis);
	mat3 m1 = mat3(pow(axisN.x, 2), axisN.x*axisN.y, axisN.x*axisN.z, axisN.x*axisN.y, pow(axisN.y, 2), axisN.y*axisN.z, axisN.x*axisN.z, axisN.y*axisN.z, pow(axisN.z, 2));
	mat3 m2 = mat3(0, axisN.z, -axisN.y, -axisN.z, 0, axisN.x, axisN.y, -axisN.x, 0);

	ret = cos(radians)*I+(1-cos(radians))*m1+sin(radians)*m2;
    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
	mat3 rotateMatrix = rotate(degrees, up);
	eye = rotateMatrix*eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.  
	mat3 mDual = mat3(0, -eye.z, eye.y, eye.z, 0, -eye.x, -eye.y, eye.x, 0);
	vec3 u = glm::normalize(mDual*up);
	mat3 rotateMatrix = rotate(degrees, u);
	eye = eye*rotateMatrix;
	up = up*rotateMatrix;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
	vec3 a = eye - center; 
	vec3 b = up;
	vec3 w = glm::normalize(a); 
	vec3 u = glm::normalize(glm::cross(b,w)); 
	vec3 v = glm::cross(w,u);
	
	ret = mat4(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0,glm::dot(u,-eye),glm::dot(v,-eye),glm::dot(w,-eye),1);
    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
	float radians = (fovy*pi)/180.f;
	float d = 1/tan(radians/2);
	float A = (-zFar+zNear)/(zFar-zNear);
	float B = (-2*zFar*zNear)/(zFar-zNear);

	ret = mat4(d/aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, A, -1, 0, 0, B, 0);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
	ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
	ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
