#include "plane.h"
#include <math.h>

Plane::Plane(const Point3 &p, const Vector3 &n, boost::shared_ptr<Material> m_):
    point(p), normal(n), m_ptr(m_),
    isGrid(true)
{
    normal.normalize();
}

bool Plane::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
    // solve parameter function on t
    double t = Vector3(ray.o, point) * normal /(ray.d * normal);

    if (t > kEpsilon) {
        tmin = t;
        sp.hit = true;
        sp.normal = normal;
        sp.hitPoint = ray.o + t * ray.d;
        // sp.color = color;
        sp.inside = false;
        sp.m = *m_ptr;
        sp.m.diffuse_color = getColor(sp.hitPoint);
        // std::cout << sp.color.toString() << std::endl;

        return true;
    } else {
        sp.hit = false;
        return false;
    }
}

bool Plane::shadow_hit(const Ray &ray, double &tmin) const
{
    double t = Vector3(ray.o, point) * normal /(ray.d * normal);
    if (t > kEpsilon) {
        tmin = t;
        return true;
    } else {
        return false;
    }
}

void Plane::shift(const Vector3 &v)
{
    point = point + v;
}

/*
 * some dirty code
 */
Color Plane::getColor(Point3 p) const
{
    /*
    Vector3 v1(point, p); // a vector in the surface
    Vector1 v2 = v1 - (v1 * normal) * normal; // an updated vector
    */
    if (isGrid){
        double zero = 0.0001;
        int x = floor(p.x * 2) ;
        int z = fabs(p.z) < zero ? floor(p.y * 2) : floor(p.z * 2);
        if ((x ^ z) & 1)
            return Color(0.1);
        else
            return Color(0.6);
    } else
        return m_ptr->diffuse_color;
}
