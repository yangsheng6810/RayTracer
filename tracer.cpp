#include "tracer.h"
#include "algorithm"
#include <cmath>
#define MAX_DEPTH 5

Tracer::Tracer()
{
}

void Tracer::setScene(const boost::shared_ptr<Scene> &s_ptr)
{
	scene_weak_ptr = boost::weak_ptr<Scene>(s_ptr);
}

double mix(double a, double b, double mix)
{
	return a * mix + b * (1 - mix);
}

Color Tracer::trace_ray(const Ray& ray, float weight) const
{
	if (weight < 0.01)
		return Color("black");
	Color environment_light = Color(0.4);
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
		Material m = sp_.m;
		if (m.isLight)
			return m.color;
		// std::cout<<m.toString()<<std::endl;
		Color ret = Color(0);
		Color environment = Color(0);
		Color diffuse = Color(0);
		Color refraction = Color(0);
        environment += environment_light * m.environment_reflect; // environment light

		// ret += environment;

		// start for direct light
		Vector3 view_vec = ray.d;// (0, 1, 0);
		Vector3 light_vec;
		Vector3 h_vec;
		double intense;
		boost::shared_ptr<Light> l_ptr;
		Color intense_p;
		if (!sp_.m.transparent || !sp_.inside)
		for(int i = 0; i != scene_ptr->lights.size(); ++i){
			l_ptr = scene_ptr->lights[i];
			Color temp = Color(0);
			int sample_number = 1;
			// to speed up for debug
			if (l_ptr->need_sample)
				sample_number = 5;
			for(int s = 0; s != sample_number; ++s){
		    	light_vec = l_ptr->get_direction(sp_);

				bool in_shadow = false;
				double t;
				double distance = sqrt(light_vec.lenSquare());
				boost::shared_ptr<BaseObject> obj;
				for(int j = 0; j != scene_ptr->objects.size(); ++j){
					obj = scene_ptr->objects.at(j);
					if (obj->shadow_hit(Ray(sp_.hitPoint, light_vec), t) && t < distance){
						in_shadow = true;
						break;
					}
				}

				if (!in_shadow){
		        	h_vec = (view_vec + (-light_vec));
		        	h_vec.normalize();
			    	intense = light_vec * sp_.normal;
		        	if (intense < 0) // don't know why
			        	intense = 0;
			    	intense_p = l_ptr->L(sp_) * intense;
					// may not produce a good effect
			    	intense_p.divide(light_vec.lenSquare());
			    	// intense_p.divide(10 * sqrt(light_vec.lenSquare()));
		        	temp += intense_p * m.Kd * (-view_vec * sp_.normal);// diffuse
		        	temp += intense_p * m.Ks * pow(h_vec * sp_.normal, m.power);// highlight
				}
			}
			/*
			if (sample_number > 1)
				temp.divide(2);
			*/
			temp.divide(sample_number);
			diffuse += temp;
		}
		// ret += diffuse;

		double ratio = ray.d * sp_.normal;
		double power0 = pow(1 - ratio, 3);
		double fresneleffect = 0.9 * power0 + 0.1;

		// start for reflect light
		Ray reflect(sp_.hitPoint, 2 * (sp_.normal * (- ray.d)) * sp_.normal + ray.d);
		reflect.d.addNoise();
		// Color reflect_c = 0.4 * trace_ray(reflect, weight * 0.1);// reflection
		Color reflect_c = m.Kd * trace_ray(reflect, weight * 0.2);// reflection
		// ret += reflect_c * fresneleffect;

		Color refract_c = Color(0);
		bool inside = false;

		// start for refract light
		if (m.transparent){
			double ior = 1.1, eta = (sp_.inside) ? ior : 1 / ior; // are we inside or outside the surface?
			Vector3 normal = sp_.normal;
			inside = sp_.inside;
			if (sp_.inside)
				normal = -normal;

			double cosi = -sp_.normal*ray.d;
			if (cosi < 0){
				// std::cout<<"ERROR!"<<std::endl;
				sp_.normal = -sp_.normal;
				cosi = -cosi;
			}
			double k = 1 - eta * eta * (1 - cosi * cosi);
			Vector3 refrdir = ray.d * eta + sp_.normal * (eta *  cosi - sqrt(k));

			// Vector3 refrdir = (-1 - 1/eta * (normal * ray.d)) * normal + ray.d;
			refrdir.normalize();

		    // Ray refraction(sp_.hitPoint, (1 - 1.0/n0)*(sp_.normal * (- ray.d)) * sp_.normal - (1.0/n0)*ray.d);
			Ray refraction(sp_.hitPoint, refrdir);
			refraction.d.addNoise();
		    // ret += 0.6 * trace_ray(refraction, weight * 0.2);
		    refract_c = m.Ks * 0.8 * trace_ray(refraction, weight * 0.2);
			// ret += refract_c * (1 - fresneleffect);
		}
		// ret.divide(10);
		// return ret * sp_.color;
		// ret.divide(1.2);
		if (inside)
		    ret = (reflect_c + refract_c) * m.r_reflect;
        else {
    		ret = environment + diffuse * m.r_diffuse + (reflect_c + refract_c) * m.r_reflect;
		    ret = ret * m.color;
        }
		return ret.toRealColor();
		// return sp_.color;
	}
	else
		return Color(0, 0, 0);// background
}
