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
	enum RenderType {GLOBAL, LIMITED, DIRECT};
	Tracer();
	void setScene(const boost::shared_ptr<Scene>& s_ptr);

	virtual Color trace_ray(const Ray& ray, float weight) const;
	boost::weak_ptr<Scene> scene_weak_ptr;
	void setRenderType(const RenderType& rt_);
	RenderType type;

};

#endif // TRACER_H
