#ifndef SHADEPACKET_H
#define SHADEPACKET_H
#include "point3.h"
#include "vector3.h"
#include "color.h"
#include "material.h"
#include <boost/smart_ptr.hpp>

class ShadePacket
{
public:
    ShadePacket();
    ShadePacket(const ShadePacket& sp);
    ShadePacket& operator = (const ShadePacket& sp);

    bool hit;
    Point3 hitPoint;
    Vector3 normal;

    bool inside;
    Material m;
};

#endif // SHADEPACKET_H
