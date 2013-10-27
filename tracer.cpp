#include "tracer.h"

Tracer::Tracer()
{
}

void Tracer::setScene(const boost::shared_ptr<Scene> &s_ptr)
{
	scene_weak_ptr = boost::weak_ptr<Scene>(s_ptr);
}

Color Tracer::trace_ray(const Ray& ray) const
{
    ShadePacket sp, sp_;
	double t, t_ = INFINITY;
	boost::shared_ptr<Scene> scene_ptr = scene_weak_ptr.lock();

	for (int i = 0; i != scene_ptr->objects.size(); i++){
		if (scene_ptr->objects[i]->hit(ray, t, sp)){
			if (t < t_){
				t_ = t;
				sp_ = sp;
			}
		}
	}

	if (t_ < INFINITY){
		return sp_.color;
	}
	else
		return Color(0, 0, 0);
}
