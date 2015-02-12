#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"

#ifdef __cplusplus
#include <string>
#include <boost/smart_ptr.hpp>

class Light;

class Material
{
public:
	Material(Color diffuse_color_ = Color(1.0, 0, 0),
	         double diffuse_intensity_ = 0.5,
	         Color specular_color_ = Color(0.8),
	         double specular_intensity_ = 0.5,
	         double hardness_ = 100,
	         bool reflect_ = false,
	         bool transparent_ = false,
	         Color emission_ = Color(0));
	Material(const Material& m);
	Material& operator = (const Material& m);
	std::string toString() const;

	Color diffuse_color, specular_color;
	double diffuse_intensity, specular_intensity, hardness;
	bool reflect;
	bool transparent;
	Color emission;
	bool isLight;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
    void* material_new_parameters(
	        void* diffuse_color,
	        double diffuse_intensity,
	        void* specular_color,
	        double specular_intensity,
	        double hardness,
	        int reflect,
	        int transparent,
	        void* emission);
#ifdef __cplusplus
}
#endif
#endif // MATERIAL_H
