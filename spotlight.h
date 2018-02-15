#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "light.h"
#include <vector>

class SpotLight : public Light
{
public:
    SpotLight(Point3 location_, Vector3 direction_, double distance_,
              Color color_, double energy_, double spot_size_);
    Vector3 get_direction(const ShadePacket &sp, int sample_n) const;
    Color L(const ShadePacket &sp) const;

private:
    Point3 location;
    Vector3 direction;
    double distance;
    Color color;
    double energy, spot_size;
    Vector3 xx, yy;
    std::vector<Point3> sampleLights;
};

#endif // SPOTLIGHT_H
