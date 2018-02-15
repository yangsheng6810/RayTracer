#include "spotlight.h"
#include <math.h>
#define SAMPLE_MAX_NUMBER 1000
inline double rand_double()
{
    return ((double)rand()/ (double)RAND_MAX);
}

SpotLight::SpotLight(Point3 location_, Vector3 direction_, double distance_,
              Color color_, double energy_, double spot_size_):
    location(location_), direction(direction_), distance(distance_),
    color(color_), energy(energy_), spot_size(spot_size_)
{
    direction.normalize();
    xx = ((fabs(direction.x) > 0.01 ? Vector3(0,1,0) : Vector3(1, 0, 0)).tensor(direction)).normalize();
    yy = direction.tensor(xx);
    Point3 loc;
    for(int i = 0; i < SAMPLE_MAX_NUMBER; ++i){
        double r1 = 2 * M_PI * rand_double();
        double r2 = rand_double();
        Vector3 dir = xx * cos(r1) + yy * sin(r1);
        loc = location + (direction + dir * r2) * 0.2;
        sampleLights.push_back(loc);
    }
    need_sample = true;
}


Vector3 SpotLight::get_direction(const ShadePacket &sp, int sample_n) const
{
    return Vector3(sp.hitPoint, sampleLights[sample_n]);
}

Color SpotLight::L(const ShadePacket &sp) const
{
    double angle_cos = cos(spot_size/2);
    Vector3 vec(location, sp.hitPoint);
    vec.normalize();
    double cos = vec * direction;
    double ratio = cos - angle_cos;
    ratio = ratio > 0 ? ratio : 0;
    ratio = 1 - pow(1 - ratio, 30);
    return (18 * energy * color * ratio);
}
