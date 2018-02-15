#ifndef BASEOBJECT_H
#define BASEOBJECT_H
#include "vector3.h"
#include "bbox.h"
#include "ray.h"
#include "shadepacket.h"

class BaseObject
{
public:
    BaseObject();
    virtual bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const = 0;
    virtual bool shadow_hit(const Ray& ray, double& tmin) const = 0;
    virtual void shift(const Vector3& v);
    virtual BBox getBBox() const
    {
        return bBox;
    }
protected:
    BBox bBox;
};

#endif // BASEOBJECT_H
