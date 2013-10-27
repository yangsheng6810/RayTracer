#include "sphere.h"
#include <math.h>
#include <iostream>

Sphere::Sphere():
    origin(), radius(1.0), color(Color(0.8, 0, 0)), kEpsilon(0.0001)
{
}

Sphere::Sphere(const Point3 &p, const double &rr):
    origin(p), radius(rr), color(Color(0.8, 0, 0)), kEpsilon(0.0001/rr)
{
}

bool Sphere::hit(const Ray& ray, double& tmin, ShadePacket& sp) const
{
    double t;
	Vector3 temp(origin, ray.o);
	double a = ray.d * ray.d;
	double b = temp * ray.d * 2.0;
	double c = temp * temp - radius * radius;
	double disc = b * b - 4.0 * a * c;

	if (disc < 0.0)
		return false;
	else {
		double e = sqrt(disc);
		double denom = a * 2.0;
		t = (-b -e) / denom;        // smaller root

		if(t > kEpsilon) {
			tmin = t;
			sp.hit = true;
			sp.normal = (temp + ray.d * t) * (1/radius);
			sp.hitPoint = ray.o + ray.d * t;
			sp.color = color;
			return true;
		}

		t = (-b + e) / denom;

		if (t > kEpsilon) {
			tmin = t;
			sp.hit = true;
			sp.normal = (temp + ray.d * t) * (1/ radius);
			sp.hitPoint = ray.o + ray.d * t;
			sp.color = color;
			return true;
		}
	}

	return false;
}


