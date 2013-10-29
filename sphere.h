#ifndef SPHERE_H
#define SPHERE_H
#include "point3.h"
#include "vector3.h"
#include "baseobject.h"
#include "shadepacket.h"

class Sphere : public BaseObject
{
public:
	Sphere();
	Sphere(const Point3& p, const double& rr);
	bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const;

	Point3 origin; // origin
	double radius;
	Color color;
	const double kEpsilon;
	double r_diffuse, r_reflect ;
	Color environment_reflect, Kd, Ks;
	double power;
};

#endif // SPHERE_H
