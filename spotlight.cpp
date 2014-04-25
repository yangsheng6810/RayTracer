#include "spotlight.h"
#include <math.h>
inline double rand_double()
{
	return ((double)rand()/ (double)RAND_MAX);
}

SpotLight::SpotLight(Point3 location_, Vector3 direction_,
	          Color color_, double energy_, double spot_size_):
    location(location_), direction(direction_),
    color(color_), energy(energy_), spot_size(spot_size_)
{
	xx = ((fabs(direction.x) > 0.01 ? Vector3(0,1,0) : Vector3(1, 0, 0)).tensor(direction)).normalize();
	yy = direction.tensor(xx);
	for(int i = 0; i < 1000; ++i)
		sampleLights.push_back(Point3(0,0,0));
}


Vector3 SpotLight::get_direction(const ShadePacket &sp, int sample_n)
{
	Point3 p = sampleLights[sample_n];
	Point3 loc;
	if (p.x == 0 && p.y == 0 && p.z == 0){
	    double r1 = 2 * M_PI * rand_double();
	    Vector3 dir = xx * cos(r1) + yy * sin(r1);
	    Vector3 unit_direction = direction;
	    unit_direction.normalize();
	    loc = location + unit_direction + dir;
		sampleLights[sample_n] = loc;
	} else {
		loc = p;
	}
	Vector3 ret = Vector3(sp.hitPoint, loc);
	// Vector3 ret = Vector3(sp.hitPoint, location);
	return ret;
}

Color SpotLight::L(const ShadePacket &sp)
{
	return (8 * energy * color);
}
