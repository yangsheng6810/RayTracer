#ifndef SHADEPACKET_H
#define SHADEPACKET_H
#include "point3.h"
#include "vector3.h"
#include "color.h"

class ShadePacket
{
public:
	ShadePacket();
	ShadePacket(const ShadePacket& sp);
	ShadePacket& operator = (const ShadePacket& sp);

	bool hit;
	Point3 hitPoint;
	Vector3 normal;
	Color color;
};

#endif // SHADEPACKET_H
