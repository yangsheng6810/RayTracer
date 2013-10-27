#ifndef PLANE_H
#define PLANE_H
#include "point3.h"
#include "vector3.h"
#include "baseobject.h"
#include "shadepacket.h"

class Plane : public BaseObject
{
public:
	Plane();
	Plane(const Point3& p, const Vector3& n, Color c = Color(0, 0.8, 0));
	bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const;

	Point3 point; // a point in the plane
	Vector3 normal; // normal
	Color color;
	static const double kEpsilon = 1e-4; // left for understanding

};

#endif // PLANE_H
