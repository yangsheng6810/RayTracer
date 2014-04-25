#include "pointlight.h"

PointLight::PointLight():
    Light(), point(Point3(0, 0, 0)), ls(1.0), color(Color(1.0))
{
}

PointLight::PointLight(Point3 point_, float ls_, Color color_):
    Light(), point(point_), ls(ls_), color(color_)
{
}

Vector3 PointLight::get_direction(const ShadePacket &sp, int sample_n)
{
	Vector3 ret = Vector3(sp.hitPoint, point);
	return ret;
}

Color PointLight::L(const ShadePacket &sp)
{
	return (ls * color);
}
