#include "transform.h"
#include <stdio.h>
 
mat3 Transform::rotate(const float degrees, const vec3& axis) {

	float radians = degrees / 180.0 * pi;
    float x = axis.x, y = axis.y, z = axis.z;
    mat3 Identity;
    mat3 aa(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);
    mat3 aStar(0.0, -z, y, z, 0.0, -x, -y, x, 0.0);
    mat3 ret = cos(radians) * Identity + (1.0 - cos(radians)) * aa +  sin(radians) * aStar;
    return ret;  
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {

    return mat4(sx, 0.0, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 0.0, sz, 0.0, 0.0, 0.0, 0.0, 1.0);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {

    return mat4(1.0, 0.0, 0.0, tx, 0.0, 1.0, 0.0, ty, 0.0, 0.0, 1.0, tz, 0.0, 0.0, 0.0, 1.0);
}

Transform::Transform()
{

}

Transform::~Transform()
{

}