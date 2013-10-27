#include "shadepacket.h"

ShadePacket::ShadePacket():
    hit(false), hitPoint(), normal(), color()
{
}

ShadePacket::ShadePacket(const ShadePacket &sp):
    hit(sp.hit), hitPoint(sp.hitPoint),
    normal(sp.normal), color(sp.color)
{
}


ShadePacket& ShadePacket::operator = (const ShadePacket& sp)
{
    hit = sp.hit;
	hitPoint = sp.hitPoint;
    normal = sp.normal;
    color = sp.color;
}
