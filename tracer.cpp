#include "tracer.h"
#include "algorithm"
#define MAX_DEPTH 5

Tracer::Tracer()
{
	light_vec = Vector3(-1, 1, -1);
	light_vec.normalize();
}

void Tracer::setScene(const boost::shared_ptr<Scene> &s_ptr)
{
	scene_weak_ptr = boost::weak_ptr<Scene>(s_ptr);
	/*
	light = boost::shared_ptr<Sphere>(new Sphere(Point3(0, 20, -30), 3));
	light->emissionColor = Color(3, 3, 3);
	light->color = Color(1, 1, 1);
	light->reflection = 0;
	light->transparency = 0;
	*/
}

double mix(double a, double b, double mix)
{
	return a * mix + b * (1 - mix);
}

Color Tracer::trace_ray(const Ray& ray, float weight) const
{
	if (weight < 0.01)
		return Color("black");
	Color environment_light = Color(1);
    ShadePacket sp, sp_;
	double t, t_ = INFINITY;
	boost::shared_ptr<Scene> scene_ptr = scene_weak_ptr.lock();
	int this_index = 1000;

	for (int i = 0; i != scene_ptr->objects.size(); i++){
		if (scene_ptr->objects[i]->hit(ray, t, sp)){
			// std::cout<<t<<", "<<sp.color.toString()<<std::endl;
			if (t < t_){
				t_ = t;
				sp_ = sp;
				this_index = i;
			}
		}
	}

	if (t_ < INFINITY){
		Color ret = Color(0);
        ret += environment_light * sp_.environment_reflect; // environment light
		Vector3 view_vec = ray.d;// (0, 1, 0);
		Vector3 h_vec = (view_vec + light_vec);
		h_vec.normalize();
		double intense = (-light_vec) * sp_.normal;
		if (intense < 0)
			intense = 0;
		Color intense_p = Color(1) * intense;
		ret += intense_p * sp_.Kd * (-view_vec * sp_.normal);// diffuse
		ret += intense_p * sp_.Ks * pow(h_vec * sp_.normal, sp_.power);// highlight
		Ray reflect(sp_.hitPoint, 2 * (sp_.normal * (- ray.d)) * sp_.normal + ray.d);
		ret = ret * sp_.color;
		ret += 0.4 * trace_ray(reflect, weight * 0.4);// reflection
		/*
		if (sp_.transparent){
			double ior = 1.5, eta = (sp_.inside) ? ior : 1 / ior; // are we inside or outside the surface?
			double cosi = -sp_.normal*ray.d;
			double k = 1 - eta * eta * (1 - cosi * cosi);
			Vector3 refrdir = ray.d * eta + sp_.normal * (eta *  cosi - sqrt(k));
			refrdir.normalize();

		    // Ray refraction(sp_.hitPoint, (1 - 1.0/n0)*(sp_.normal * (- ray.d)) * sp_.normal - (1.0/n0)*ray.d);
			Ray refraction(sp_.hitPoint, refrdir);
		    ret += 0.8 * trace_ray(refraction, weight * 0.4);
		}
		*/
		// ret.divide(10);
		// return ret * sp_.color;
		return ret;
		// return sp_.color;
	}
	else
		return Color(0, 0, 0);// background
}
