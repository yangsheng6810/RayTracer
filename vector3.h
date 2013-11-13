#ifndef VECTOR3_H
#define VECTOR3_H
#include <string>
#include <iostream>
#include "point3.h"

class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& v);
	Vector3(float xx, float yy, float zz);
	Vector3(const Point3& p1, const Point3& p2);
    Vector3& operator +=(const Vector3& v);
	Vector3 operator -(const Vector3& v) const;
	Vector3 operator -() const;
	float operator *(const Vector3& v) const;
    Vector3& operator *=(const float& v);
	Vector3& operator =(const Vector3& v);
    float dot(const Vector3& v) const;
    Vector3 tensor(const Vector3& v) const;
	void normalize();
	void addNoise();
	double lenSquare() const;

	std::string toString() const;

	float x, y, z;
};

Vector3 operator +(const Vector3& v1, const Vector3& v2);
Point3 operator +(const Vector3& v, const Point3& p);
Point3 operator +(const Point3& p, const Vector3& v);
Vector3 operator *(const Vector3& v, const float& f);
Vector3 operator *(const float& f, const Vector3 &v);

#endif // VECTOR3_H
