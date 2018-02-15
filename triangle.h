#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <boost/smart_ptr.hpp>
#include "baseobject.h"
class TriangleMesh;

class Triangle : public BaseObject
{
public:
	Triangle(const boost::shared_ptr<TriangleMesh>& obj_ptr,
	         size_t v0_, size_t v1_, size_t v2_, const Vector3& normal_, size_t material_index_);
	bool hit(const Ray& ray, double& tmin, ShadePacket& sp) const;
	bool shadow_hit(const Ray& ray, double& tmin) const;
	void shift(const Vector3& v);
	size_t v0, v1, v2;
	Vector3 normal;
	size_t material_index;
private:
	boost::weak_ptr<TriangleMesh> w_object_ptr;
};

#endif // TRIANGLE_H
