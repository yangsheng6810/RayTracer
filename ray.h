#ifndef RAY_H
#define RAY_H
#include "string"
#include "iostream"
#include "point3.h"
#include "vector3.h"

class Ray
{
public:
	Ray();
	Ray(const Point3& oo, const Vector3& dd);
	Ray(const Ray& r);
	Ray& operator = (const Ray& r);
	void normalize();
    std::string toString() const;

	Point3 o;
	Vector3 d;
};

#endif // RAY_H
