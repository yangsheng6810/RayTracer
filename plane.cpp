#include "plane.h"

Plane::Plane():
    point(), normal(), color(Color(0, 0.8, 0))
{

}

Plane::Plane(const Point3 &p, const Vector3 &n, Color c):
    point(p), normal(n), color(c)
{
}

bool Plane::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
	// solve parameter function on t
	double t = Vector3(ray.o, point) * normal /(ray.d * normal);

	if (t > kEpsilon) {
		tmin = t;
		sp.hit = true;
		sp.normal = normal;
		sp.color = color;
		sp.hitPoint = ray.o + t * ray.d;

		return true;
	} else {
		sp.hit = false;
		return false;
	}
}
