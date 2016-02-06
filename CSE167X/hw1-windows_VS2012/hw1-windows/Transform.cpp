// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
	const float rad = degrees * pi / 180;
	const auto cosTheta = glm::cos(rad);
	const auto sinTheta = glm::sin(rad);
	const mat3 uCross(axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.x * axis.y, axis.y * axis.y, axis.y * axis.z, axis.x * axis.z, axis.y * axis.z, axis.z * axis.z);
	const mat3 uX(0, axis.z, -axis.y, -axis.z, 0, axis.x, axis.y, -axis.x, 0);

	const mat3 rotation = cosTheta * mat3(1.0) + sinTheta*uX + (1 - cosTheta)*(uCross);
	return rotation;
}

// Transforms the camera left around the "crystal ball" interface
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

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up)
{
	vec3 w, u, v;
	constructCoordinateFrame(eye, up, w, u, v);

	eye = rotate(-degrees, u) * eye;
	up = rotate(-degrees, u)*up;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up)
{
	vec3 w, u, v;
	constructCoordinateFrame(eye, up, w, u, v);

	mat4 lookAtRotMat(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, 0, 0, 0, 1);
	mat4 lookAtTransMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -eye.x, -eye.y, -eye.z, 1);

	return lookAtRotMat * lookAtTransMat;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
