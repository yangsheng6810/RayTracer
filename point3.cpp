#include <sstream>
#include <cassert>
#include "point3.h"

Point3::Point3()
{
    x = y = z = 0;
}

Point3::Point3(const Point3& p)
{
    x = p.x;
    y = p.y;
    z = p.z;
}

Point3::Point3(float xx, float yy, float zz)
{
    x = xx;
    y = yy;
    z = zz;
}

Point3& Point3::operator = (const Point3& p)
{
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

float& Point3::operator[] (const int& index)
{
    assert(index >= 0 && index < 3);
    switch (index){
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            // should not visit this
            return x;
            std::cout<<"error! in Point3::operator[]"<<std::endl;
    }
}

float Point3::get(const int& index) const
{
    assert(index >= 0 && index < 3);
    switch (index){
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            // should not visit this
            return x;
            std::cout<<"error! in Point3::operator[]"<<std::endl;
    }
}

std::string Point3::toString() const
{
    std::ostringstream strs;
    strs<<"("<<x<<", "<<y<<", "<<z<<")";
    std::string str = strs.str();
    return str;
}
