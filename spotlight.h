#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "light.h"

class SpotLight : public Light
{
public:
	SpotLight(Point3 location_, Vector3 direction_,
	          Color color_, double energy_, double spot_size_);
	Vector3 get_direction(const ShadePacket &sp) const;
	Color L(const ShadePacket &sp);

private:
	Color color;
	Point3 location;
	Vector3 direction;
	Vector3 xx, yy;
	double energy, spot_size;
};

#endif // SPOTLIGHT_H
