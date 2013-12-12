#include "trianglemesh.h"
#include "triangle.h"

Triangle::Triangle(const boost::shared_ptr<TriangleMesh>& obj_ptr,
                   size_t v0_, size_t v1_, size_t v2_, const Vector3& normal_):
    object_ptr(obj_ptr), v0(v0_), v1(v1_), v2(v2_), normal(normal_)
{
	bBox.extends(object_ptr->vertices[v0]);
	bBox.extends(object_ptr->vertices[v1]);
	bBox.extends(object_ptr->vertices[v2]);
}

bool Triangle::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
	sp.normal = normal;
    return object_ptr->hitFace(v0, v1, v2, ray, tmin, sp);
}

bool Triangle::shadow_hit(const Ray &ray, double &tmin) const
{
	return object_ptr->shadow_hitFace(v0, v1, v2, ray, tmin);
}

void Triangle::shift(const Vector3 &v)
{
}
