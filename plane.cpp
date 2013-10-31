#include "plane.h"
#include <math.h>

Plane::Plane():
    point(), normal(), color(Color(0, 0.8, 0)),
    Kd(Color(0.8)), Ks(Color(0.4)), isGrid(true)
{
	environment_reflect = Color(0.1);
	power = 100;
	normal.normalize();

}

Plane::Plane(const Point3 &p, const Vector3 &n, Color c):
    point(p), normal(n), color(c),
    Kd(Color(0.8)), Ks(Color(0.4)), isGrid(true)
{
	environment_reflect = Color(0.1);
	power = 100;
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
		sp.color = getColor(sp.hitPoint);
		// std::cout << sp.color.toString() << std::endl;
		sp.reflect = false;
		sp.environment_reflect = environment_reflect;
		sp.Kd = Kd;
		sp.Ks = Ks;
		sp.power = power;
		sp.transparent = false;

		return true;
	} else {
		sp.hit = false;
		return false;
	}
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
		    return Color("black");
	    else
		    return Color("white");
	} else
		return Color(0.4);
}
