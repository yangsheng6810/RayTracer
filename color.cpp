#include <sstream>
#include "color.h"

Color::Color()
{
	r = g = b = 0;
}

Color::Color(const Color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
}

Color::Color(float rr, float gg, float bb)
{
	r = rr;
	g = gg;
	b = bb;
}

Color& Color::operator = (const Color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
}

std::string Color::toString() const
{
	std::ostringstream strs;
	strs<<"("<<r<<", "<<g<<", "<<b<<")";
	std::string str = strs.str();
	return str;
}
