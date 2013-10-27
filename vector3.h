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
	Vector3(double xx, double yy, double zz);
	Vector3(const Point3& p1, const Point3& p2);
	Vector3 operator +(const Vector3& v) const;
    Vector3& operator +=(const Vector3& v);
	Vector3 operator -(const Vector3& v) const;
	Vector3 operator -() const;
	Vector3 operator *(const Vector3& v) const;
	Vector3 operator *(const double& f) const;
    Vector3& operator *=(const Vector3& v);
    Vector3& operator *=(const double& v);
	Vector3& operator =(const Vector3& v);
    double dot(const Vector3& v) const;
    Vector3 tensor(const Vector3& v) const;

	std::string toString() const;
	friend std::ostream & operator << (std::ostream &os, const Vector3& v);

	double x, y, z;
};

#endif // VECTOR3_H
