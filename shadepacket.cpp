#include "shadepacket.h"

ShadePacket::ShadePacket():
    hit(false), hitPoint(),
    normal(), m(Material())
{
}

ShadePacket::ShadePacket(const ShadePacket &sp):
    hit(sp.hit), hitPoint(sp.hitPoint),
    normal(sp.normal), inside(sp.inside),
    m(sp.m)
{
}


ShadePacket& ShadePacket::operator = (const ShadePacket& sp)
{
    hit = sp.hit;
    hitPoint = sp.hitPoint;
    normal = sp.normal;
    inside = sp.inside;
    m = sp.m;
    return *this;
}
