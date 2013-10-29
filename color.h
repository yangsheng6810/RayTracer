#ifndef COLOR_H
#define COLOR_H
#include <string>

class Color
{
public:
	Color();
	Color(const Color& c);
	Color(float shade);
	Color(float rr, float gg, float bb);
	Color(std::string name);
	Color& operator = (const Color& p);
	void divide(float f);
	Color& operator +=(const Color& c);
	Color toRealColor();

	std::string toString() const;

	float r, g, b;
};
Color operator *(float f, const Color& c);
Color operator *(const Color& c, float f);
Color operator *(const Color& c1, const Color& c2);
Color operator +(const Color& c1, const Color& c2);

#endif // COLOR_H
