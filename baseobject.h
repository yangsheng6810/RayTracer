#ifndef BASEOBJECT_H
#define BASEOBJECT_H
#include "ray.h"
#include "shadepacket.h"

class BaseObject
{
public:
	BaseObject();
	virtual bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const = 0;
};

#endif // BASEOBJECT_H
