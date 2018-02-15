#include "parallel.h"

Parallel::Parallel():
    Light(), direction(Vector3()), ls(1.0), color(Color(1.0))
{
}

Parallel::Parallel(Vector3 dir_, float ls_, Color color_):
    Light(), direction(dir_), ls(ls_), color(color_)
{
}

Vector3 Parallel::get_direction(const ShadePacket &sp, int sample_n)
{
    return direction;
}

Color Parallel::L(const ShadePacket &sp)
{
    return (ls * color);
}
