#include "scene.h"
#include "tracer.h"
#include <boost/make_shared.hpp>

using namespace boost;

Scene::Scene():
	output(new Output()),
	tracer_ptr(new Tracer())
{
	objects.push_back(shared_ptr<BaseObject>(new Sphere(Point3(0, 0, 1), 1)));
	objects.push_back(shared_ptr<BaseObject>(new Plane(Point3(0, 0, 0), Vector3(0, 0, 1), Color(0.2, 0.2, 0.2))));// ground
	objects.push_back(shared_ptr<BaseObject>(new Plane(Point3(0, 3, 0), Vector3(0, -1, 0))));// background
}

void Scene::renderScene()
{
	Color color;
	Ray ray;
	double yw = -4.0;
	double x, z;
	int width = 800, height = 600;

	tracer_ptr->setScene(shared_from_this());

	ray.o = Point3(0, -5, 1);

	for(int r = 0; r < width; r++)
		for(int c = 0; c < height; c++){
			x = 2.0/width *(r - 0.5 * width);
			z = 2.0/width *(c - 0.5 * width) + 1.0;
			ray.d = Vector3(ray.o, Point3(x, yw, z));
			ray.normalize();
			color = tracer_ptr->trace_ray(ray);
			output->addColor(color, r, c);
		}
	output->writePic();
}
