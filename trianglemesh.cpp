#include "trianglemesh.h"
#include <sstream>

TriangleMesh::TriangleMesh(boost::shared_ptr<Material> m_):
    m_ptr(m_), kEpsilon(0.001)
{
	x_min = y_min = z_min =  10000;
	x_max = y_max = z_max = -10000;
	smooth = true;
	// smooth = false;
}

bool TriangleMesh::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
	double tmin_ = 1e10, tmin_temp;
	ShadePacket sp_, sp_temp;
	for(size_t i = 0; i != faces.size(); ++i)
		if (hitFace(i, ray, tmin_temp, sp_temp) && tmin_temp < tmin_){
			tmin_ = tmin_temp;
			sp_ = sp_temp;
		}
	if (tmin_ < 1e10){
		tmin = tmin_;
		sp = sp_;
		return true;
	} else
    	return false;
}

bool TriangleMesh::shadow_hit(const Ray &ray, double &tmin) const
{
	double tmin_ = 1e10, tmin_temp;
	for(size_t i = 0; i != faces.size(); ++i)
		if (shadow_hitFace(i, ray, tmin_temp) && tmin_temp < tmin_){
			tmin_ = tmin_temp;
		}
	if (tmin_ < 1e10){
		tmin = tmin_;
		return true;
	} else
    	return false;
}

bool TriangleMesh::shadow_hitFace(int index, const Ray &ray, double &tmin) const
{
	Point3 v0 = vertices[faces[index].v0];
	Point3 v1 = vertices[faces[index].v1];
	Point3 v2 = vertices[faces[index].v2];

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

bool TriangleMesh::hitFace(int index, const Ray &ray, double &tmin, ShadePacket &sp) const
{
	Point3 v0 = vertices[faces[index].v0];
	Point3 v1 = vertices[faces[index].v1];
	Point3 v2 = vertices[faces[index].v2];

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
		sp.normal = (1 - beta - gamma ) * normals[faces[index].v0] + beta * normals[faces[index].v1] + gamma * normals[faces[index].v2];
		sp.normal.normalize();
	}else
    	sp.normal = face_normals[index];
	sp.m = *m_ptr;
	sp.hitPoint = ray.o + t * ray.d;

	return true;
}

void TriangleMesh::shift(const Vector3 &v)
{
	for(int i = 0; i != vertices.size(); ++i)
		vertices[i] = vertices[i] + v;
}

void TriangleMesh::addVertice(const Point3 &p, const Vector3 &v)
{
	vertices.push_back(p);
	normals.push_back(v);
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

void TriangleMesh::addFace(int v0, int v1, int v2)
{
	faces.push_back(Face(v0, v1, v2));
	Vector3 n = Vector3(vertices[v1], vertices[v0]).tensor(Vector3(vertices[v2], vertices[v1]));
	n.normalize();
	face_normals.push_back(n);
}

std::string TriangleMesh::toString() const
{
	std::ostringstream strs;
	for(int i = 0; i < vertices.size(); ++i)
		strs<<"["<<i<<vertices[i].toString()<<"]"<<std::endl;
	strs<<std::endl;
	for(int i = 0; i < faces.size(); ++i)
	    strs<<"("<<faces[i].v0<<", "<<faces[i].v1<<", "<<faces[i].v2<<")"<<std::endl;
	std::string str = strs.str();
	return str;
}
