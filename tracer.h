#ifndef TRACER_H
#define TRACER_H
#include "color.h"
#include "vector3.h"
#include "scene.h"
#include "ray.h"
#include "shadepacket.h"
#include <boost/smart_ptr.hpp>

class Tracer
{
public:
	Tracer();
	void setScene(const boost::shared_ptr<Scene>& s_ptr);

	virtual Color trace_ray(const Ray& ray, float weight) const;
	boost::weak_ptr<Scene> scene_weak_ptr;
    Vector3 light_vec;
};

#endif // TRACER_H
