#include <sstream>
#include <cstdio>
#include "color.h"
#include "get_object.h"

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

Color::Color(double rr, double gg, double bb)
{
	r = rr;
	g = gg;
	b = bb;
}

Color::Color(double shade)
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
	return *this;
}


Color& Color::operator +=(const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

std::string Color::toString() const
{
	std::ostringstream strs;
	strs<<"("<<r<<", "<<g<<", "<<b<<")";
	std::string str = strs.str();
	return str;
}

void Color::divide(double f)
{
	r /= f;
	g /= f;
	b /= f;
}

Color Color::toRealColor()
{
	return Color(r > 1 ? 1.0 : r,
	             g > 1 ? 1.0 : g,
	             b > 1 ? 1.0 : b);
}

Color operator *(double f, const Color& c)
{
	return Color(c.r * f, c.g * f, c.b * f);
}

Color operator *(const Color& c, double f)
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


Color blend(const Color& c1, const Color& c2)
{
	return Color(c1.r + c2.r - c1.r * c2.r,
				 c1.g + c2.g - c1.g * c2.g,
				 c1.b + c2.b - c1.b * c2.b);
}

extern "C"{
    void* color_new(){
		Color* c = new Color();
		return ((void *) c);
	}
	void* color_new_pointer(void *c__){
		Color* c = new Color(GET_COLOR(c__));
		return ((void *)c);
	}
	void* color_new_double(double shade){
		Color* c = new Color(shade);
		return ((void *)c);
	}
	void* color_new_rgb(double r, double g, double b){
		Color* c = new Color(r,g,b);
		return ((void *)c);
	}
	void color_print(void *c__){
		printf("here!!\n");
		printf(GET_COLOR(c__).toString().c_str());
		printf("\n");
	}
}
