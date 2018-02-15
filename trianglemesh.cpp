#include "trianglemesh.h"
#include "sahkdtree.h"
#include <boost/make_shared.hpp>
#include <sstream>

TriangleMesh::TriangleMesh(bool smooth_):
    BaseObject(),tree_ptr(new SAHKDTree()), kEpsilon(0.0001), smooth(smooth_)
{
	bBox = BBox();
	x_min = y_min = z_min =  10000;
	x_max = y_max = z_max = -10000;
}

TriangleMesh::~TriangleMesh()
{
	vertices.clear();
	normals.clear();
	for (size_t i = 0; i < materials.size(); ++i)
		materials[i].reset();
	materials.clear();
	for (size_t i = 0; i < triangles.size(); ++i)
		triangles[i].reset();
	triangles.clear();
}

void TriangleMesh::addMaterial(const boost::shared_ptr<Material>& m_)
{
	materials.push_back(m_);
}

bool TriangleMesh::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
	return tree_ptr->hit(ray, tmin, sp);
	/*
	double tmin_ = 1e10, tmin_temp;
	ShadePacket sp_, sp_temp;
	for(size_t i = 0; i < triangles.size(); ++i)
		if (triangles[i]->hit(ray, tmin_temp, sp_temp) && tmin_temp < tmin_){
			tmin_ = tmin_temp;
			sp_ = sp_temp;
		}
	if (tmin_ < 1e10){
		tmin = tmin_;
		sp = sp_;
		return true;
	} else
    	return false;
	*/
}

bool TriangleMesh::shadow_hit(const Ray &ray, double &tmin) const
{
	ShadePacket sp;
	return tree_ptr->hit(ray, tmin, sp);
	/*
	double tmin_ = 1e10, tmin_temp;
	for(size_t i = 0; i != triangles.size(); ++i)
		if (triangles[i]->shadow_hit(ray, tmin_temp) && tmin_temp < tmin_){
			tmin_ = tmin_temp;
		}
	if (tmin_ < 1e10){
		tmin = tmin_;
		return true;
	} else
    	return false;
		*/
}

bool TriangleMesh::shadow_hitFace(size_t v0_, size_t v1_, size_t v2_, const Ray &ray, double &tmin) const
{
	Point3 v0 = vertices[v0_];
	Point3 v1 = vertices[v1_];
	Point3 v2 = vertices[v2_];

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom = 1.0/(a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return false;

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
		return false;

	if (beta + gamma > 1.0)
		return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon)
		return false;

	tmin = t;

	return true;
}

bool TriangleMesh::hitFace(size_t v0_, size_t v1_, size_t v2_, const Ray &ray,
                           double &tmin, ShadePacket &sp, size_t material_index) const
{
	Point3 v0 = vertices[v0_];
	Point3 v1 = vertices[v1_];
	Point3 v2 = vertices[v2_];

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom = 1.0/(a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return false;

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
		return false;

	if (beta + gamma > 1.0)
		return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon)
		return false;

	tmin = t;
	sp.hit = true;
	if (smooth){
		sp.normal = (1 - beta - gamma ) * normals[v0_] + beta * normals[v1_] + gamma * normals[v2_];
		sp.normal.normalize();
	}
	// else is assigned in Triangle::hit
	// sp.m = *m_ptr;
	sp.m = *(materials[material_index]);
	sp.inside = (sp.normal * ray.d) > 0;
	sp.hitPoint = ray.o + t * ray.d;

	return true;
}

void TriangleMesh::shift(const Vector3 &v)
{
	for(size_t i = 0; i != vertices.size(); ++i)
		vertices[i] = vertices[i] + v;
}

void TriangleMesh::addVertice(const Point3 &p, const Vector3 &v)
{
	vertices.push_back(p);
	normals.push_back(v);
	bBox.extends(p);
	updateLimit(p);
}

void TriangleMesh::updateLimit(const Point3 &p)
{
	if (p.x < x_min)
		x_min = p.x;
	if (p.x > x_max)
		x_max = p.x;

	if (p.y < y_min)
		y_min = p.y;
	if (p.y > y_max)
		y_max = p.y;

	if (p.z < z_min)
		z_min = p.z;
	if (p.z > z_max)
		z_max = p.z;
}

void TriangleMesh::addFace(size_t v0, size_t v1, size_t v2, size_t material_index)
{
	// faces.push_back(Face(v0, v1, v2));
	Vector3 n = Vector3(vertices[v1], vertices[v0]).tensor(Vector3(vertices[v1], vertices[v2]));
	n.normalize();
	triangles.push_back(boost::shared_ptr<Triangle>(new Triangle(shared_from_this(), v0, v1, v2, n, material_index)));
}

std::string TriangleMesh::toString() const
{
	std::ostringstream strs;
	for(size_t i = 0; i < vertices.size(); ++i)
		strs<<"["<<i<<vertices[i].toString()<<"]"<<std::endl;
	strs<<std::endl;
	for(size_t i = 0; i < triangles.size(); ++i){
	    strs<<"("<<boost::static_pointer_cast<Triangle>(triangles[i])->v0<<
		      ", "<<boost::static_pointer_cast<Triangle>(triangles[i])->v1<<
		      ", "<<boost::static_pointer_cast<Triangle>(triangles[i])->v2<<")"<<std::endl;
	}
	std::string str = strs.str();
	return str;
}

void TriangleMesh::finishObject()
{
	tree_ptr->build(triangles);
}
