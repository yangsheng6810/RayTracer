#ifndef PARALLEL_H
#define PARALLEL_H
#include "light.h"

class Parallel : public Light
{
public:
    Parallel();
    Parallel(Vector3 dir_, float ls_, Color color_);
    Vector3 get_direction(const ShadePacket &sp, int sample_n);
    Color L(const ShadePacket &sp);

private:
    Vector3 direction;
    float ls;
    Color color;
};

#endif // PARALLEL_H
