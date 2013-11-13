#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H
#include "point3.h"
#include "vector3.h"
#include "baseobject.h"
#include "shadepacket.h"
#include "material.h"
#include <boost/smart_ptr.hpp>
#include <vector>

class Face {
public:
	Face(int v0_, int v1_, int v2_): v0(v0_), v1(v1_), v2(v2_){}
	int v0, v1, v2;
};

class TriangleMesh : public BaseObject
{
public:
	TriangleMesh(boost::shared_ptr<Material> m_ = boost::shared_ptr<Material>());
	void addVertice(const Point3& p, const Vector3& v);
	void addFace(int v0, int v1, int v2);
	bool hit(const Ray &ray, double &tmin, ShadePacket &sp) const;
	bool shadow_hit(const Ray &ray, double &tmin) const;
	void shift(const Vector3& v);

	bool shadow_hitFace(int index, const Ray &ray, double &tmin) const;
	bool hitFace(int i, const Ray &ray, double &tmin, ShadePacket &sp) const;

	std::string toString() const;
	std::vector<Point3> vertices;
	std::vector<Face> faces;
	std::vector<Vector3> normals;
	std::vector<Vector3> face_normals;
private:
	void updateLimit(const Point3& p);
	float x_min, x_max, y_min, y_max, z_min, z_max;
	boost::shared_ptr<Material> m_ptr;
	double kEpsilon;
	bool smooth;

};

#endif // TRIANGLEMESH_H
