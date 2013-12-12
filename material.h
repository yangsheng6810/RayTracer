#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"
#include <string>
#include <boost/smart_ptr.hpp>

class Light;

class Material
{
public:
	Material(Color c_ = Color(0.8, 0, 0),
	         Color Kd_ = Color(0.8),
	         Color Ks_ = Color(0.7),
	         Color environment_reflect_ = Color(0.1),
			 double r_diffuse_ = 1.0,
	         double r_reflect_ = 0,
	         double power_ = 100,
	         bool reflect_ = false,
	         bool transparent_ = false,
	         bool isLight_ = false);
	Material(const Material& m);
	Material& operator = (const Material& m);
	std::string toString() const;

	double r_diffuse, r_reflect;
	Color color, environment_reflect, Kd, Ks;
	double power;
	bool transparent;
	bool reflect;
	bool isLight;
	boost::shared_ptr<Light> light_ptr;
};

#endif // MATERIAL_H
