#include <sstream>
#include "ray.h"

Ray::Ray()
{
	o = Point3(0, 0, 0);
	d = Vector3(0, 0, 0);
}

Ray::Ray(const Point3 &oo, const Vector3 &dd)
{
	o = oo;
	d = dd;
}

Ray::Ray(const Ray &r)
{
	o = r.o;
	d = r.d;
}

Ray& Ray::operator =(const Ray &r)
{
	o = r.o;
	d = r.d;
}

std::string Ray::toString() const
{
	std::ostringstream strs;
	strs<<"Ray[" << o.toString() << d.toString() << "]";
	std::string str = strs.str();
	return str;
}
