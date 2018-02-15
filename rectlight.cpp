#include "rectlight.h"
#include <cstdlib>

RectLight::RectLight(boost::shared_ptr<Rectangle> rect_ptr_, float ls_, Color color_):
    Light(), ls(ls_), color(color_), rect_ptr(rect_ptr_)
{
    rect_ptr->m_ptr->isLight = true;
    rect_ptr->m_ptr->diffuse_color = ls * color;
    need_sample = true;
    srand(12458);
}

inline float rand_float()
{
    return ((float)rand()/ (float)RAND_MAX);
}

Vector3 RectLight::get_direction(const ShadePacket &sp, int sample_n) const
{
    Point3 p = rect_ptr->p0;
#ifdef WITH_SAMPLE_NUMBER
    p = p + rand_float() * rect_ptr->a;
    p = p + rand_float() * rect_ptr->b;
#else
    p = p + 0.5 * rect_ptr->a;
    p = p + 0.5 * rect_ptr->b;
#endif
    Vector3 ret = Vector3(sp.hitPoint, p); //.normalize();
    return ret;
}

Color RectLight::L(const ShadePacket &sp) const
{
    return (ls * color);
}
