#include <math.h>
#include <float.h>
#include "bbox.h"
#include <sstream>


BBox::BBox()
{
}

BBox::BBox(const Point3& min_, const Point3& max_):
    min(min_), max(max_)
{
}

bool BBox::intersect(const Ray& r, float& t0, float& t1) const
{
	// maybe need debug!!
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	if (r.inv_d.x >= 0) {
	    tmin = (min.x - r.o.x) * r.inv_d.x;
        tmax = (max.x - r.o.x) * r.inv_d.x;
	} else {
	    tmin = (max.x - r.o.x) * r.inv_d.x;
        tmax = (min.x - r.o.x) * r.inv_d.x;
	}

	if (r.inv_d.y >= 0) {
        tymin = (min.y - r.o.y) * r.inv_d.y;
        tymax = (max.y - r.o.y) * r.inv_d.y;
	} else {
        tymin = (max.y - r.o.y) * r.inv_d.y;
        tymax = (min.y - r.o.y) * r.inv_d.y;
	}
	if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;
	if (r.inv_d.z >= 0){
        tzmin = (min.z - r.o.z) * r.inv_d.z;
        tzmax = (max.z - r.o.z) * r.inv_d.z;
	} else {
        tzmin = (max.z - r.o.z) * r.inv_d.z;
        tzmax = (min.z - r.o.z) * r.inv_d.z;
	}
	if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
      tmin = tzmin;
    if (tzmax < tmax)
      tmax = tzmax;
	t0 = tmin;
	t1 = tmax;
    return true;
}


void BBox::extends(const BBox &bbox)
{
	min.x = min.x < bbox.min.x ? min.x : bbox.min.x;
	min.y = min.y < bbox.min.y ? min.y : bbox.min.y;
	min.z = min.z < bbox.min.z ? min.z : bbox.min.z;

	max.x = max.x > bbox.max.x ? max.x : bbox.max.x;
	max.y = max.y > bbox.max.y ? max.y : bbox.max.y;
	max.z = max.z > bbox.max.z ? max.z : bbox.max.z;
}

void BBox::extends(const Point3 &p)
{
	min.x = min.x < p.x ? min.x : p.x;
	min.y = min.y < p.y ? min.y : p.y;
	min.z = min.z < p.z ? min.z : p.z;

	max.x = max.x > p.x ? max.x : p.x;
	max.y = max.y > p.y ? max.y : p.y;
	max.z = max.z > p.z ? max.z : p.z;
}

BBox BBox::split(const int& _dim, const float& _splitVal)
{
	BBox ret;
	ret.min = min;
	ret.max = max;
	ret.min[_dim] = _splitVal;

	max[_dim] = _splitVal;
	return ret;
}

float BBox::getSurfaceArea(void)
{
	float length = fabs(max.x-min.x);
    float width = fabs(max.y-min.y);
    float height = fabs(max.z-min.z);

    return 2.0f*(length*width+length*height+width*height);
}

float BBox::getArea(void)
{
	float length = fabs(max.x-min.x);
    float width = fabs(max.y-min.y);
    float height = fabs(max.z-min.z);

    return (length+width+height)/3.0f;
}

const Vector3 BBox::diagonal() const
{
	return Vector3(min, max);
}

std::string BBox::toString() const
{
	std::ostringstream strs;
	strs<<"["
	    <<"min = "<<min.toString()
	    <<"max = "<<max.toString()
	    <<"]";
	std::string str = strs.str();
	return str;
}
