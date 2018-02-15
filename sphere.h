#ifndef SPHERE_H
#define SPHERE_H
#include "point3.h"
#include "vector3.h"
#include "baseobject.h"
#include "shadepacket.h"
#include "material.h"
#include <boost/smart_ptr.hpp>

class Sphere : public BaseObject
{
public:
    Sphere();
    Sphere(const Point3& p = Point3(), const double& rr = 1,
           boost::shared_ptr<Material> m = boost::shared_ptr<Material>());
    bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const;
    bool shadow_hit(const Ray &ray, double &tmin) const;
    void shift(const Vector3& v);

    Point3 origin; // origin
    double radius;
    boost::shared_ptr<Material> m_ptr;

    Color color;
    const double kEpsilon;
};

#endif // SPHERE_H
