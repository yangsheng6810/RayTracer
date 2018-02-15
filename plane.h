#ifndef PLANE_H
#define PLANE_H
#include "point3.h"
#include "vector3.h"
#include "baseobject.h"
#include "shadepacket.h"
#include <boost/smart_ptr.hpp>

class Plane : public BaseObject
{
public:
    Plane(const Point3& p = Point3(0, 0, 0), const Vector3& n = Vector3(0, 0, 1),
          boost::shared_ptr<Material> m = boost::shared_ptr<Material>());
    bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const;
    bool shadow_hit(const Ray &ray, double &tmin) const;
    void shift(const Vector3& v);
    Color getColor(Point3 p) const;

    Point3 point; // a point in the plane
    Vector3 normal; // normal
    // Color color;
    boost::shared_ptr<Material> m_ptr;
    bool isGrid;
    static const double kEpsilon = 1e-4; // left for understanding

};

#endif // PLANE_H
