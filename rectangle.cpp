#include "rectangle.h"

Rectangle::Rectangle(const Point3& p_, const Vector3& a_,
                     const Vector3& b_, boost::shared_ptr<Material> m_):
    p0(p_), a(a_), b(b_), m_ptr(m_)
{
	a_lenSquare = a.lenSquare();
	b_lenSquare = b.lenSquare();
	normal = a.tensor(b).normalize();
	// normal.normalize();
	bBox.extends(p0);
	bBox.extends(p0 + a);
	bBox.extends(p0 + b);
	bBox.extends(p0 + a + b);
	// std::cout<<normal.toString()<<std::endl;
}

bool Rectangle::hit(const Ray &ray, double &tmin, ShadePacket &sp) const
{
	float t = Vector3(ray.o, p0) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return false;

	Point3 p = ray.o + t * ray.d;
	Vector3 d = Vector3(p0, p);

	float ddota = d * a;

	if (ddota < 0.0 || ddota > a_lenSquare)
		return false;

	float ddotb = d * b;

	if (ddotb < 0.0 || ddotb > b_lenSquare)
		return false;

	tmin = t;
	sp.hit = true;
	sp.normal = normal * ray.d < 0 ? normal : -normal;
	// sp.normal = normal;
	sp.inside = false;
	sp.hitPoint = p;
	sp.m = *m_ptr;
	// sp.m.Kd = Color(0.8);
	// std::cout<<"hit rectangle!"<<std::endl;

	return true;
}

bool Rectangle::shadow_hit(const Ray &ray, double &tmin) const
{
	// potential problem!!
	if (m_ptr->emission.r > 0)
		return false;
	float t = Vector3(ray.o, p0) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return false;

	Point3 p = ray.o + t * ray.d;
	Vector3 d = Vector3(p0, p);

	float ddota = d * a;

	if (ddota < 0.0 || ddota > a_lenSquare)
		return false;

	float ddotb = d * b;

	if (ddotb < 0.0 || ddotb > b_lenSquare)
		return false;

	tmin = t;

	return true;
}


void Rectangle::shift(const Vector3 &v)
{
	p0 = p0 + v;
}
