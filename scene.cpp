#include "scene.h"
#include "tracer.h"
#include <boost/make_shared.hpp>
#include <cstdlib>
#include <math.h>

using namespace boost;

Scene::Scene():
    width(640),
    height(480),
	output(new Output(640, 480)),
	tracer_ptr(new Tracer())
{
	buildScene();
}

inline float rand_float()
{
	return ((float)rand()/ (float)RAND_MAX);
}

/**
 * @brief buildScene
 * build the scene for rendering
 */
void Scene::buildScene()
{
	objects.clear();
	shared_ptr<Sphere> s;
	for (int i = -1; i <= 1; i++ )
		for (int j = 0; j <= 2; j++){
	        s = shared_ptr<Sphere>(new Sphere(Point3(i, 0, j), 0.4));
	        objects.push_back(shared_ptr<BaseObject>(s));
		}
	s = shared_ptr<Sphere>(new Sphere(Point3(0, -1, 1), 1));
	s->color = Color(0, 0.8, 0);
	objects.push_back(shared_ptr<BaseObject>(s));
	/*
	s = shared_ptr<Sphere>(new Sphere(Point3(-2, 0, 3), 1.5));
	objects.push_back(shared_ptr<BaseObject>(s));

    s = shared_ptr<Sphere>(new Sphere(Point3(2, -1, 1.2), 0.6));
	objects.push_back(shared_ptr<BaseObject>(s));
	*/

	objects.push_back(shared_ptr<BaseObject>(new Plane(Point3(0, 0, 0), Vector3(0, 0, 1), Color(0.2))));// ground
	objects.push_back(shared_ptr<BaseObject>(new Plane(Point3(0, 3, 0), Vector3(0, -1, 0), Color(0.7))));// background
}

void Scene::renderScene()
{
	Color color;
	Ray ray;
	double yw = -3.0;
	double inv_w = 1.0/width, inv_h = 1.0/height;
	double fov = 60, aspectratio = (double) width / height;
	double angle = tan(M_PI * 0.5 * fov / (double)180);
	double inv = 8 * inv_w;
	int number = 20;

	tracer_ptr->setScene(shared_from_this());

	ray.o = Point3(0, -10, 1);

	srand(10000);

	for(int z = 0; z < height; z++)
		for(int x = 0; x < width; x++){
			/*
			// double xx = inv *(x - 0.5 * width);
			// double zz = inv *(0.5 * height - z) + 0.9;
			double xx = (2 * ((x + 0.5) * inv_w) - 1) * angle * aspectratio;
			double zz = (1 - 2 * ((z + 0.5) * inv_h)) * angle;
			// ray.d = Vector3(ray.o, Point3(xx, 1, zz));
			ray.d = Vector3(Point3(), Point3(xx, 4, zz));
			ray.normalize();
    		color = tracer_ptr->trace_ray(ray, 1);
			*/

			color = Color(0, 0, 0);
			for(int q = 0; q < number; q++) {
				double xx = (2 * ((x + rand_float()) * inv_w) - 1) * angle * aspectratio;
				double zz = (1 - 2 * ((z + rand_float()) * inv_h)) * angle;
				// x = inv *(r - 0.5 * width + rand_float() - 0.5);
				// z = inv *(c - 0.5 * height + rand_float() - 0.5) + 1.0;
		    	// ray.d = Vector3(ray.o, Point3(x, yw, z));
			    ray.d = Vector3(Point3(), Point3(xx, 4, zz));
			    ray.normalize();
    			// color += tracer_ptr->trace_ray(ray, 0);
				color += tracer_ptr->trace_ray(ray, 1);
			}
			color.divide(number);
			output->addColor(color, x, z);
		}
	output->writePic();
}
