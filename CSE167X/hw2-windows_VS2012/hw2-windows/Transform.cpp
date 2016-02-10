// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	const vec3 normaxis = glm::normalize(axis);
	const float rad = degrees * pi / 180;
	const auto cosTheta = glm::cos(rad);
	const auto sinTheta = glm::sin(rad);
	const mat3 uCross(normaxis.x * normaxis.x, normaxis.x * normaxis.y, normaxis.x * normaxis.z, 
		normaxis.x * normaxis.y, normaxis.y * normaxis.y, normaxis.y * normaxis.z,
		normaxis.x * normaxis.z, normaxis.y * normaxis.z, normaxis.z * normaxis.z);

	const mat3 uX(0, normaxis.z, -normaxis.y, -normaxis.z, 0, normaxis.x, normaxis.y, -normaxis.x, 0);

	const mat3 rotation = cosTheta * mat3(1.0) + sinTheta*uX + (1 - cosTheta)*(uCross);
	return rotation;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	eye = rotate(degrees, up) * eye;
}

void constructCoordinateFrame(vec3 a, vec3 b, vec3& w, vec3& u, vec3& v)
{
	w = glm::normalize(a);
	u = glm::normalize(glm::cross(b, w));
	v = glm::cross(w, u);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	vec3 w, u, v;
	constructCoordinateFrame(eye, up, w, u, v);

	eye = rotate(-degrees, u) * eye;
	up = rotate(-degrees, u)*up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	vec3 w, u, v;
	constructCoordinateFrame(eye, up, w, u, v);

	mat4 lookAtRotMat(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, 0, 0, 0, 1);
	mat4 lookAtTransMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -eye.x, -eye.y, -eye.z, 1);

	return lookAtRotMat * lookAtTransMat;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	fovy = glm::radians(fovy);
	float f = 1 / tan(fovy / 2);
	mat4 ret(f / aspect, 0, 0, 0, 0, f, 0, 0, 0, 0, (zFar + zNear) / (zNear - zFar), -1, 0, 0, (2 * zFar*zNear) / (zNear - zFar), 0);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
	mat4 ret(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	mat4 ret(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
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
