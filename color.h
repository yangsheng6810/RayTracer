#ifndef COLOR_H
#define COLOR_H

#ifdef __cplusplus
#include <string>

class Color
{
public:
	Color();
	Color(const Color& c);
	Color(double shade);
	Color(double rr, double gg, double bb);
	Color(std::string name);
	Color& operator = (const Color& p);
	void divide(double f);
	Color& operator +=(const Color& c);
	Color toRealColor();

	std::string toString() const;

	double r, g, b;
};
Color operator *(double f, const Color& c);
Color operator *(const Color& c, double f);
Color operator *(const Color& c1, const Color& c2);
Color operator +(const Color& c1, const Color& c2);
Color blend(const Color& c1, const Color& c2);
#endif

#ifdef __cplusplus
extern "C"{
#endif
    void* color_new();
	void* color_new_pointer(void*);
	void* color_new_double(double);
	void* color_new_rgb(double, double, double);
	void color_print(void*);
#ifdef __cplusplus
}
#endif

#endif // COLOR_H
