#include "trianglemesh.h"
#include "triangle.h"

Triangle::Triangle(const boost::shared_ptr<TriangleMesh>& obj_ptr,
                   size_t v0_, size_t v1_, size_t v2_,
                   const Vector3& normal_, size_t material_index_):
    BaseObject(), v0(v0_), v1(v1_), v2(v2_),
    normal(normal_), material_index(material_index_), w_object_ptr(obj_ptr)
{
    bBox = BBox();
    boost::shared_ptr<TriangleMesh> object_ptr = w_object_ptr.lock();
    bBox.extends(object_ptr->vertices[v0]);
    bBox.extends(object_ptr->vertices[v1]);
    bBox.extends(object_ptr->vertices[v2]);
}

bool Triangle::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
    sp.normal = normal;
    boost::shared_ptr<TriangleMesh> object_ptr = w_object_ptr.lock();
    return object_ptr->hitFace(v0, v1, v2, ray, tmin, sp, material_index);
}

bool Triangle::shadow_hit(const Ray &ray, double &tmin) const
{
    boost::shared_ptr<TriangleMesh> object_ptr = w_object_ptr.lock();
    return object_ptr->shadow_hitFace(v0, v1, v2, ray, tmin);
}

void Triangle::shift(const Vector3 &v)
{
}
