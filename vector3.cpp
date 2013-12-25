#include <sstream>
#include "vector3.h"
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include <cassert>

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

inline float rand_float()
{
	return ((float)rand()/ (float)RAND_MAX);
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

Vector3 &Vector3::normalize()
{
	double n = sqrt(lenSquare());
	x = x/n;
	y = y/n;
	z = z/n;
	return *this;
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

float& Vector3::operator[] (const int& index)
{
	assert(index >= 0 && index < 3);
	switch (index){
		case 0:
	        return x;
            break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			// should not visit this
			std::cout<<"error! in Point3::operator[]"<<std::endl;
	}
}

float Vector3::dot(const Vector3& v) const
{
	return (float)x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::tensor(const Vector3& v) const
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

double Vector3::lenSquare() const
{
	return x * x + y * y + z * z;
}

void Vector3::addNoise()
{
	/*
	Vector3 noise(rand_float(), rand_float(), rand_float());

	x += noise.x / 10000;
	y += noise.y / 10000;
	z += noise.z / 10000;
	*/
}

std::string Vector3::toString() const
{
	std::ostringstream strs;
	strs<<"("<<x<<", "<<y<<", "<<z<<")";
	std::string str = strs.str();
	return str;
}
