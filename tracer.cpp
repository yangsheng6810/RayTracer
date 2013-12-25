#include "tracer.h"
#include "algorithm"
#include <ctime>
#include <cmath>
#define MAX_DEPTH 5
#define RATE 0.4

Tracer::Tracer()
{
	srand(time(NULL));
	type = Tracer::DIRECT;
}

inline double rand_double()
{
	return ((double)rand()/ (double)RAND_MAX);
}

void Tracer::setScene(const boost::shared_ptr<Scene> &s_ptr)
{
	scene_weak_ptr = boost::weak_ptr<Scene>(s_ptr);
}

double mix(double a, double b, double mix)
{
	return a * mix + b * (1 - mix);
}

Color Tracer::diffuse_global(const ShadePacket& sp_, const double& weight) const
{
	Color diffuse = Color(0);
	// Color   f = sp_.m.color;
	// double p = std::max(std::max(f.r, f.g), f.b);

	if (sp_.m.emission.r > 0)
		return sp_.m.emission;

	double r1 = 2 * M_PI * rand_double(),
	       r2 = rand_double(),
	       r2s = sqrt(r2);
	Vector3 w = sp_.normal,
	        u = ((fabs(w.x) > 0.01 ? Vector3(0,1,0) : Vector3(1, 0, 0)).tensor(w)).normalize(),
	        // v = (w.tensor(u)).normalize();
	        v = w.tensor(u);
	Vector3 direction = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
	Ray r = Ray(sp_.hitPoint, direction);
	diffuse += trace_ray(r, weight * RATE);// reflection
	return diffuse;
}

Color Tracer::diffuse_limited() const
{
	if (sp_.m.emission.r > 0)
		return sp_.m.emission;
	Color diffuse = Color(0);

	// start for direct light
	Vector3 view_vec = ray.d;// (0, 1, 0);
	Vector3 light_vec;
	Vector3 h_vec;
	double intense;
	boost::shared_ptr<Light> l_ptr;
	Color intense_p;

	for(int i = 0; i != scene_ptr->lights.size(); ++i){
		l_ptr = scene_ptr->lights[i];
		Color temp = Color(0);
		int sample_number = 1;
		// to speed up for debug
#ifdef WITH_SAMPLE_NUMBER
		if (l_ptr->need_sample)
			sample_number = 5;
#endif
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
				h_vec = (view_vec + (-light_vec)).normalize();
				// h_vec.normalize();
				intense = light_vec * sp_.normal;
				if (intense < 0) // don't know why
					intense = 0;
				intense_p = l_ptr->L(sp_) * intense;
				// may not produce a good effect
				intense_p.divide(light_vec.lenSquare());
				// intense_p.divide(10 * sqrt(light_vec.lenSquare()));
				temp += intense_p * sp_.m.Kd * (-view_vec * sp_.normal);// diffuse
				temp += intense_p * sp_.m.Ks * pow(h_vec * sp_.normal, sp_.m.power);// highlight
			}
		}
		// if (sample_number > 1)
		// 	temp.divide(2);
		temp.divide(sample_number);
		diffuse += temp;
	}
}

Color diffuse_direct() const
{
	if (sp_.m.emission.r > 0)
		return sp_.m.emission;
	Color diffuse = Color(0);

	// start for direct light
	Vector3 view_vec = ray.d;// (0, 1, 0);
	Vector3 light_vec;
	Vector3 h_vec;
	double intense;
	boost::shared_ptr<Light> l_ptr;
	Color intense_p;

	for(int i = 0; i != scene_ptr->lights.size(); ++i){
		l_ptr = scene_ptr->lights[i];
		Color temp = Color(0);
		h_vec = (view_vec + (-light_vec)).normalize();
		// h_vec.normalize();
		intense = light_vec * sp_.normal;
		if (intense < 0) // don't know why
			intense = 0;
		intense_p = l_ptr->L(sp_) * intense;
		// may not produce a good effect
		intense_p.divide(light_vec.lenSquare());
		// intense_p.divide(10 * sqrt(light_vec.lenSquare()));
		temp += intense_p * sp_.m.Kd * (-view_vec * sp_.normal);// diffuse
		temp += intense_p * sp_.m.Ks * pow(h_vec * sp_.normal, sp_.m.power);// highlight
		diffuse += temp;
	}
	return diffuse;
}

Color Tracer::trace_ray(const Ray& ray, float weight) const
{
	if (weight < 0.01)
		return Color("black");
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
		/*
		if (sp_.m.isLight)
			return sp_.m.color;
		*/
		// std::cout<<sp_.m.toString()<<std::endl;
		Color ret = Color(0);
		Color environment = Color(0);
		Color diffuse = Color(0);
		/*
		Color refraction = Color(0);
        // environment += environment_light * sp_.m.environment_reflect; // environment light

		// ret += environment;

		// start for direct light
		Vector3 view_vec = ray.d;// (0, 1, 0);
		Vector3 light_vec;
		Vector3 h_vec;
		double intense;
		boost::shared_ptr<Light> l_ptr;
		Color intense_p;
		*/
		if (!sp_.m.transparent || !sp_.inside){
			/*
			switch (type) {
			case DIRECT:
			    diffuse += diffuse_direct(sp_);
				break;
			case GLOBAL:
			    diffuse += diffuse_global(sp_, weight);
				break;
			case LIMITED:
		        diffuse += diffuse_limited(sp_);
				break;
			default:
			    diffuse += diffuse_no_shadow(sp_);
			}
			*/

			Color   f = sp_.m.color;

			// double p = std::max(std::max(f.r, f.g), f.b);

			if (sp_.m.emission.r > 0)
				return sp_.m.emission;

			/*
			if (weight < 0.01)
				if (rand_double() < p)
			        f = f * (1/p);
			    else
			        return sp_.m.emission;
					*/
			//for(int sample = 0; sample < 5; sample++){
			double r1 = 2 * M_PI * rand_double(),
			        r2 = rand_double(),
			       r2s = sqrt(r2);
			Vector3 w = sp_.normal,
			        u = ((fabs(w.x) > 0.01 ? Vector3(0,1,0) : Vector3(1, 0, 0)).tensor(w)).normalize(),
			        // v = (w.tensor(u)).normalize();
			        v = w.tensor(u);
			Vector3 direction = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
			Ray r = Ray(sp_.hitPoint, direction);
    		diffuse += trace_ray(r, weight * RATE);// reflection
			// }
			//diffuse.divide(5);

		/*
		for(int i = 0; i != scene_ptr->lights.size(); ++i){
			l_ptr = scene_ptr->lights[i];
			Color temp = Color(0);
			int sample_number = 1;
			// to speed up for debug
#ifdef WITH_SAMPLE_NUMBER
			if (l_ptr->need_sample)
				sample_number = 5;
#endif
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
		        	h_vec = (view_vec + (-light_vec)).normalize();
		        	// h_vec.normalize();
			    	intense = light_vec * sp_.normal;
		        	if (intense < 0) // don't know why
			        	intense = 0;
			    	intense_p = l_ptr->L(sp_) * intense;
					// may not produce a good effect
			    	intense_p.divide(light_vec.lenSquare());
			    	// intense_p.divide(10 * sqrt(light_vec.lenSquare()));
		        	temp += intense_p * sp_.m.Kd * (-view_vec * sp_.normal);// diffuse
		        	temp += intense_p * sp_.m.Ks * pow(h_vec * sp_.normal, sp_.m.power);// highlight
				}
			}
			// if (sample_number > 1)
			// 	temp.divide(2);
			temp.divide(sample_number);
			diffuse += temp;
		}
		*/
		}
		// ret += diffuse;

		double ratio = ray.d * sp_.normal;
		double power0 = pow(1 - ratio, 3);
		double fresneleffect = 0.9 * power0 + 0.1;

		Color reflect_c = Color(0);
		// start for reflect light
		if (sp_.m.reflect){
		    Ray reflect(sp_.hitPoint, 2 * (sp_.normal * (- ray.d)) * sp_.normal + ray.d);
		    // reflect.d.addNoise();
		    // Color reflect_c = 0.4 * trace_ray(reflect, weight * 0.1);// reflection
		    reflect_c = trace_ray(reflect, weight * RATE);// reflection
		    // ret += reflect_c * fresneleffect;
		}

		Color refract_c = Color(0);
		bool inside = false;

		// start for refract light
		if (sp_.m.transparent){
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
			// not implemented, and may not need
			// refraction.d.addNoise();
		    // ret += 0.6 * trace_ray(refraction, weight * RATE);
		    refract_c = trace_ray(refraction, weight * RATE);
			// ret += refract_c * (1 - fresneleffect);
		}
		// ret.divide(10);
		// return ret * sp_.color;
		// ret.divide(1.2);
		if (inside)
		    ret = sp_.m.emission + reflect_c * sp_.m.r_reflect
			      + refract_c * (1 - sp_.m.r_diffuse - sp_.m.r_reflect);
        else {
			ret = sp_.m.emission + environment + diffuse * sp_.m.r_diffuse + reflect_c * sp_.m.r_reflect
			      + refract_c * (1 - sp_.m.r_diffuse - sp_.m.r_reflect);
		    ret = ret * sp_.m.color;
        }
		// return ret.toRealColor();
		return ret;
		// return sp_.color;
	}
	else
		return Color(0, 0, 0);// background
}

void Tracer::setRenderType(const RenderType& rt_)
{
	type = rt_;
}
