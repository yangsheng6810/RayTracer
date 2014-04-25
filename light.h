#ifndef LIGHT_H
#define LIGHT_H
#include "vector3.h"
#include "color.h"
#include "shadepacket.h"

class Light
{
public:
	Light();
	virtual Vector3 get_direction(const ShadePacket& sp, int sample_n) = 0;
	virtual Color L(const ShadePacket& sp) = 0;
	bool need_sample;

protected:
	bool shadows;
};

#endif // LIGHT_H
