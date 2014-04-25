#ifndef RECTLIGHT_H
#define RECTLIGHT_H
#include "light.h"
#include "rectangle.h"
#include <boost/smart_ptr.hpp>

class RectLight : public Light
{
public:
	RectLight(boost::shared_ptr<Rectangle> rect_ptr_,
			float ls_, Color color_);
	Vector3 get_direction(const ShadePacket &sp, int sample_n);
	Color L(const ShadePacket &sp);

private:
	float ls;
	Color color;
	boost::shared_ptr<Rectangle> rect_ptr;
};

#endif // RECTLIGHT_H
