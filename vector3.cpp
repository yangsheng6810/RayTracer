#include <sstream>
#include "vector3.h"
#include <math.h>

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3::Vector3(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

Vector3::Vector3(const Point3 &p1, const Point3 &p2)
{
	x = p2.x - p1.x;
	y = p2.y - p1.y;
	z = p2.z - p1.z;
}

Vector3& Vector3::operator +=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator -(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

float Vector3::operator *(const Vector3& v) const
{
	return (float)(x * v.x + y * v.y + z * v.z);
}

void Vector3::normalize()
{
	double n = sqrt(x*x + y*y + z*z);
	x = x/n;
	y = y/n;
	z = z/n;
}

Vector3 operator +(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Point3 operator +(const Vector3& v, const Point3& p)
{
	return Point3(v.x + p.x, v.y + p.y, v.z + p.z);
}

Point3 operator +(const Point3& p, const Vector3& v)
{
	return Point3(v.x + p.x, v.y + p.y, v.z + p.z);
}

Vector3 operator *(const Vector3 &v, const float& f)
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}

Vector3 operator *(const float& f, const Vector3 &v)
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}

Vector3& Vector3::operator *=(const float& f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3& Vector3::operator =(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

float Vector3::dot(const Vector3& v) const
{
	return (float)x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::tensor(const Vector3& v) const
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

std::string Vector3::toString() const
{
	std::ostringstream strs;
	strs<<"("<<x<<", "<<y<<", "<<z<<")";
	std::string str = strs.str();
	return str;
}
