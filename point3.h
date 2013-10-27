#ifndef POINT3_H
#define POINT3_H
#include <string>
#include <iostream>

class Point3
{
public:
	Point3();
	Point3(const Point3& p);
	Point3(float xx, float yy, float zz);
	Point3& operator = (const Point3& p);

	std::string toString() const;

	float x, y, z;
};

#endif // POINT3_H
