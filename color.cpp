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

Color::Color(float shade)
{
	r = g = b = shade;
}

Color::Color(std::string name)
{
	if (name == "black"){
		r = g = b = 0;
	}
	if (name == "white"){
		r = g = b = 1.0;
	}
	if (name == "red"){
		r = 1.0;
		g = b = 0;
	}
	if (name == "green"){
		r = b = 0;
		g = 1.0;
	}
	if (name == "blue"){
		r = g = 0;
		b = 1.0;
	}
}

Color& Color::operator = (const Color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
}


Color& Color::operator +=(const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
}

std::string Color::toString() const
{
	std::ostringstream strs;
	strs<<"("<<r<<", "<<g<<", "<<b<<")";
	std::string str = strs.str();
	return str;
}

void Color::divide(float f)
{
	r /= f;
	g /= f;
	b /= f;
}

Color Color::toRealColor()
{
	return Color(r > 1 ? 1 : r,
	             g > 1 ? 1 : g,
	             b > 1 ? 1 : b);
}

Color operator *(float f, const Color& c)
{
	return Color(c.r * f, c.g * f, c.b * f);
}

Color operator *(const Color& c, float f)
{
	return Color(c.r * f, c.g * f, c.b * f);
}

Color operator *(const Color& c1, const Color& c2)
{
	return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}

Color operator +(const Color& c1, const Color& c2)
{
	return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
