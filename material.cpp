#include "material.h"
#include <sstream>

Material::Material(Color diffuse_color_,
         double diffuse_intensity_,
         Color specular_color_,
         double specular_intensity_,
         double hardness_,
         bool reflect_,
         bool transparent_, Color emission_):
    diffuse_color(diffuse_color_),
    specular_color(specular_color_),
    diffuse_intensity(diffuse_intensity_),
    specular_intensity(specular_intensity_),
    hardness(hardness_),
    reflect(reflect_),
    transparent(transparent_),
    emission(emission_)
{}

Material::Material(const Material &m):
    diffuse_color(m.diffuse_color),
    specular_color(m.specular_color),
    diffuse_intensity(m.diffuse_intensity),
    specular_intensity(m.specular_intensity),
    hardness(m.hardness),
    reflect(m.reflect),
    transparent(m.transparent),
    emission(m.emission),
    isLight(m.isLight)
{
}

Material& Material::operator =(const Material& m)
{
	emission = m.emission;
	diffuse_color = m.diffuse_color;
    diffuse_intensity = m.diffuse_intensity;
    specular_color = m.specular_color;
    specular_intensity = m.specular_intensity;
    hardness = m.hardness;
    reflect = m.reflect;
    transparent = m.transparent;
	isLight = m.isLight;
	return *this;
}


std::string Material::toString() const
{
	std::ostringstream strs;
	strs <<"(color = "<<diffuse_color.toString()
	    <<"reflect = "<<reflect
	    <<"transparent = "<<transparent
		<<"isLight = "<<isLight
	    <<"emission = "<< emission.toString();
	std::string str = strs.str();
	return str;
}
