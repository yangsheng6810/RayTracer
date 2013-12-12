#include <sstream>
#include "ray.h"

Ray::Ray()
{
	o = Point3(0, 0, 0);
	d = Vector3(0, 0, 0);
	inv_d = Vector3(1/d.x, 1/d.y, 1/d.z);
}

Ray::Ray(const Point3 &oo, const Vector3 &dd)
{
	o = oo;
	d = dd;
	inv_d = Vector3(1/d.x, 1/d.y, 1/d.z);
}

Ray::Ray(const Ray &r)
{
	o = r.o;
	d = r.d;
	inv_d = r.inv_d;
}

Ray& Ray::operator =(const Ray &r)
{
	o = r.o;
	d = r.d;
	inv_d = r.inv_d;
}

void Ray::normalize()
{
	d.normalize();
	inv_d = Vector3(1/d.x, 1/d.y, 1/d.z);
}

std::string Ray::toString() const
{
	std::ostringstream strs;
	strs<<"Ray[" << o.toString() << d.toString() << "]";
	std::string str = strs.str();
	return str;
}
