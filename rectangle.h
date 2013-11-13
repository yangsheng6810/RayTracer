#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "point3.h"
#include "vector3.h"
#include "baseobject.h"
#include "shadepacket.h"
#include <boost/smart_ptr.hpp>

class Rectangle : public BaseObject
{
public:
	Rectangle(const Point3& p_ = Point3(0, 0, 0),
	          const Vector3& a_ = Vector3(1, 0, 0),
	          const Vector3& b_ = Vector3(0, 1, 0),
	          boost::shared_ptr<Material> m_ = boost::shared_ptr<Material>());
	bool hit(const Ray &ray, double &tmin, ShadePacket &sp) const;
	bool shadow_hit(const Ray &ray, double &tmin) const;
	void shift(const Vector3& v);

	Point3 p0; // an angle point
	Vector3 a, b; // edge vector
	Vector3 normal;
	boost::shared_ptr<Material> m_ptr;
private:
	static const double kEpsilon = 1e-5; // left for understanding
	double a_lenSquare, b_lenSquare;
};

#endif // RECTANGLE_H
