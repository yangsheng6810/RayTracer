#include "sphere.h"
#include <math.h>
#include <iostream>

Sphere::Sphere():
    origin(), radius(1.0), color(Color(0.8, 0, 0)), kEpsilon(0.001),// emissionColor(Color(0, 0, 0)),
    // transparency(0), reflection(0.8)
    Kd(Color(0.8)), Ks(Color(0.7))
{
	environment_reflect = Color(0.1);
	power = 100;
}

Sphere::Sphere(const Point3 &p, const double &rr):
    origin(p), radius(rr), color(Color(0.8, 0, 0)), kEpsilon(0.001/rr),//  emissionColor(Color(0, 0, 0)),
    // transparency(0), reflection(0.8)
    Kd(Color(0.8)), Ks(Color(0.7))
{
	environment_reflect = Color(0.1);
	// environment_reflect = color;
	power = 100;
}

bool Sphere::hit(const Ray& ray, double& tmin, ShadePacket& sp) const
{
	/*
	Vector3 l = Vector3(ray.o, origin);
	double tca = l * ray.d;
	if (tca < 0)
		return false;
	double d2 = l * l - tca * tca;
	double radius2 = radius * radius;
	if (d2 > radius2)
		return false;
	double thc = sqrt(radius2 - d2);
	double tt = tca - thc;
	if (tt > kEpsilon){
		tmin = tt;
		sp.hit = true;
		sp.color = color;
		return true;
	}
	tt = tca + thc;
	if (tt > kEpsilon){
		tmin = tt;
		sp.hit = true;
		sp.color = color;
		return true;
	}
	return true;
	*/


    double t;
	// Vector3 temp = Vector3(ray.o, origin);
	Vector3 temp = Vector3(origin, ray.o);
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
			sp.normal.normalize();
			sp.hitPoint = ray.o + ray.d * t;
			sp.color = color;
			sp.reflect = true;
			sp.environment_reflect = environment_reflect;
			sp.Kd = Kd;
			sp.Ks = Ks;
			sp.power = power;
			sp.transparent = true;
			sp.inside = false;
			return true;
		}

		t = (-b + e) / denom;

		if (t > kEpsilon) {
			tmin = t;
			sp.hit = true;
			sp.normal = (temp + ray.d * t) * (1/ radius);
			sp.normal.normalize();
			sp.hitPoint = ray.o + ray.d * t;
			sp.color = color;
			sp.reflect = true;
			sp.environment_reflect = environment_reflect;
			sp.Kd = Kd;
			sp.Ks = Ks;
			sp.power = power;
			sp.inside = true;
			sp.transparent = true;
			return true;
		}
	}

	return false;
}


