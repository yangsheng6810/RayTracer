#ifndef COLOR_H
#define COLOR_H
#include <string>

class Color
{
public:
	Color();
	Color(const Color& c);
	Color(float rr, float gg, float bb);
	Color& operator = (const Color& p);

	std::string toString() const;

	float r, g, b;
};

#endif // COLOR_H
