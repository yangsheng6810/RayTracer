#include "shadepacket.h"

ShadePacket::ShadePacket():
    hit(false), hitPoint(),
    normal(), color(),
    reflect(false), environment_reflect(Color(0))
{
}

ShadePacket::ShadePacket(const ShadePacket &sp):
    hit(sp.hit), hitPoint(sp.hitPoint),
    normal(sp.normal), color(sp.color),
    reflect(sp.reflect), environment_reflect(sp.environment_reflect),
    Kd(sp.Kd), Ks(sp.Ks), power(sp.power), inside(sp.inside), transparent(sp.transparent)
{
}


ShadePacket& ShadePacket::operator = (const ShadePacket& sp)
{
    hit = sp.hit;
	hitPoint = sp.hitPoint;
    normal = sp.normal;
    color = sp.color;
	reflect = sp.reflect;
	environment_reflect = sp.environment_reflect;
	Kd = sp.Kd;
	Ks = sp.Ks;
	power = sp.power;
	inside = sp.inside;
	transparent = sp.transparent;
}
