#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(Point3 point_, float ls_, Color color_);
	Vector3 get_direction(const ShadePacket &sp, int sample_n) const;
	Color L(const ShadePacket &sp) const;

private:
	Point3 point;
	float ls;
	Color color;
};

#endif // POINTLIGHT_H
