#include "plane.h"

Plane::Plane():
    point(), normal(), color(Color(0, 0.8, 0)),
    Kd(Color(0.8)), Ks(Color(0.4))
{
	environment_reflect = Color(0.1);
	power = 100;

}

Plane::Plane(const Point3 &p, const Vector3 &n, Color c):
    point(p), normal(n), color(c),
    Kd(Color(0.8)), Ks(Color(0.4))
{
	environment_reflect = Color(0.1);
	power = 100;
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
		sp.reflect = false;
		sp.environment_reflect = environment_reflect;
		sp.Kd = Kd;
		sp.Ks = Ks;
		sp.power = power;
		sp.transparent = false;

		return true;
	} else {
		sp.hit = false;
		return false;
	}
}
