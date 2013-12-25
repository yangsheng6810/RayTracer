#include "material.h"
#include <sstream>

Material::Material(Color c_, Color Kd_, Color Ks_, Color environment_reflect_, double r_diffuse_, double r_reflect_,
                   double power_, bool reflect_, bool transparent_, Color emission_):
    color(c_), environment_reflect(environment_reflect_),
    Kd(Kd_), Ks(Ks_), power(power_),
    reflect(reflect_), transparent(transparent_),
    isLight(false), light_ptr(boost::shared_ptr<Light>()),
    r_diffuse(r_diffuse_), r_reflect(r_reflect_),
    emission(emission_)
{
}

Material::Material(const Material &m):
    color(m.color), environment_reflect(m.environment_reflect),
    Kd(m.Kd), Ks(m.Ks), power(m.power),
    reflect(m.reflect), transparent(m.transparent),
    isLight(m.isLight), light_ptr(m.light_ptr),
    r_reflect(m.r_reflect), r_diffuse(m.r_diffuse),
    emission(m.emission)
{
}

Material& Material::operator =(const Material& m)
{
    r_diffuse = m.r_diffuse;
	r_reflect = m.r_reflect;
	color = m.color;
	environment_reflect = m.environment_reflect;
	Kd = m.Kd;
	Ks = m.Ks;
	power = m.power;
	reflect = m.reflect;
	transparent = m.transparent;
	isLight = m.isLight;
	light_ptr = m.light_ptr;
	emission = m.emission;
}


std::string Material::toString() const
{
	std::ostringstream strs;
	strs <<"(color = "<<color.toString()
	    <<"environment_reflect = " << environment_reflect.toString()
	    <<"Kd = " << Kd.toString()
	    <<"Ks = "<<Ks.toString()
	    <<"reflect = "<<reflect
	    <<"transparent = "<<transparent
		<<"isLight = "<<isLight
	    <<"emission = "<< emission.toString();
	std::string str = strs.str();
	return str;
}
