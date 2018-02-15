#ifndef COLOR_H
#define COLOR_H
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

#endif // COLOR_H
