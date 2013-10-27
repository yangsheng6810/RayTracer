#include <sstream>
#include "point3.h"

Point3::Point3()
{
	x = y = z = 0;
}

Point3::Point3(const Point3& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

Point3::Point3(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

Point3& Point3::operator = (const Point3& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

std::string Point3::toString() const
{
	std::ostringstream strs;
	strs<<"("<<x<<", "<<y<<", "<<z<<")";
	std::string str = strs.str();
	return str;
}