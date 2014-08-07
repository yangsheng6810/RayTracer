#ifndef TRACER_H
#define TRACER_H
#include "color.h"
#include "vector3.h"
#include "ray.h"
#include "shadepacket.h"
#include "constants.h"
#include <boost/smart_ptr.hpp>

class Scene;

class Tracer
{
public:
	enum RenderType {GLOBAL, LIMITED, DIRECT};
	Tracer();
	~Tracer();
	void setScene(const boost::shared_ptr<Scene>& s_ptr);

	virtual Color trace_ray(const Ray& ray, float weight, int sample_n) const;
	boost::weak_ptr<Scene> scene_weak_ptr;
	// boost::shared_ptr<Scene> scene_ptr;
	void setRenderType(const RenderType& rt_);
	RenderType type;
private:
    Color diffuse_direct(const Ray &ray, const ShadePacket& sp_) const;
    Color diffuse_global(const ShadePacket& sp_, const double& weight) const;
    Color diffuse_limited(const Ray &ray, const ShadePacket& sp_, int sample_n) const;


};

#endif // TRACER_H
