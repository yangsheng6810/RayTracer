#include <sstream>
#include "vector3.h"

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

Vector3 Vector3::operator +(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
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

Vector3 Vector3::operator *(const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3& Vector3::operator *=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3 Vector3::operator *(const float& f) const
{
	return Vector3(f * x, f * y, f * z);
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
