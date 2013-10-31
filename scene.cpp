#include "scene.h"
#include "tracer.h"
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include "boost/threadpool.hpp"
#include <cstdlib>
#include <math.h>

using namespace boost;

Scene::Scene(int w, int h):
    width(w),
    height(h),
	output(new Output(w, h)),
	tracer_ptr(new Tracer())
{
	buildScene();
	inv_w = 1.0/width;
	inv_h = 1.0/height;
	fov = 30;
	aspectratio = (double) width / height;
	angle = tan(M_PI * 0.5 * fov / (double)180);
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
	for (int i = -2; i <= 2; i++ )
		for (int j = 0; j <= 4; j++){
	        s = shared_ptr<Sphere>(new Sphere(Point3((double)i/2.0, 0, (double)j/2.0 + 0.2), 0.2));
	        objects.push_back(shared_ptr<BaseObject>(s));
		}
	s = shared_ptr<Sphere>(new Sphere(Point3(0, -3, 1), 0.6));
	s->color = Color(1, 1, 1);
	objects.push_back(shared_ptr<BaseObject>(s));
	/*
	s = shared_ptr<Sphere>(new Sphere(Point3(-2, 0, 3), 1.5));
	objects.push_back(shared_ptr<BaseObject>(s));

    s = shared_ptr<Sphere>(new Sphere(Point3(2, -1, 1.2), 0.6));
	objects.push_back(shared_ptr<BaseObject>(s));
	*/

	shared_ptr<Plane> p;
	p = shared_ptr<Plane>(new Plane(Point3(0, 0, 0), Vector3(0, 0, 1), Color(0.2)));// ground
	p->isGrid = false;
	objects.push_back(shared_ptr<BaseObject>(p));
	objects.push_back(shared_ptr<BaseObject>(new Plane(Point3(0, 3, 0), Vector3(0, -1, 0), Color(0.7))));// background
}

void Scene::renderScene()
{
	Color color;
	Ray ray;
	double yw = -3.0;
	double inv_w = 1.0/width, inv_h = 1.0/height;
	double fov = 30, aspectratio = (double) width / height;
	double angle = tan(M_PI * 0.5 * fov / (double)180);
	int number = 20;

	tracer_ptr->setScene(shared_from_this());

	ray.o = Point3(0, -20, 1);

	srand(10000);

	for(int z = 0; z < height; z++)
		for(int x = 0; x < width; x++){
			color = traceRay(ray, z, x);
			/*
			double xx = (2 * ((x + 0.5) * inv_w) - 1) * angle * aspectratio;
			double zz = (1 - 2 * ((z + 0.5) * inv_h)) * angle;
			// ray.d = Vector3(ray.o, Point3(xx, 1, zz));
			ray.d = Vector3(Point3(), Point3(xx, 4, zz));
			ray.normalize();
    		color = tracer_ptr->trace_ray(ray, 1);
			*/

			/*
			color = Color(0, 0, 0);
			for(int q = 0; q < number; q++) {
				double xx = (2 * ((x + rand_float()) * inv_w) - 1) * angle * aspectratio;
				double zz = (1 - 2 * ((z + rand_float()) * inv_h)) * angle;
		    	// ray.d = Vector3(ray.o, Point3(x, yw, z));
			    ray.d = Vector3(Point3(), Point3(xx, 4, zz));
			    ray.normalize();
    			// color += tracer_ptr->trace_ray(ray, 0);
				color += tracer_ptr->trace_ray(ray, 1);
			}
			color.divide(number);
			*/
			output->addColor(color, x, z);
		}
	output->writePic();
}

void Scene::renderSceneThread()
{
	origin = Point3(0, -20, 1);
	tracer_ptr->setScene(shared_from_this());
	int row_number = 3;
	int col_number = 4;
	int tile_height = height / row_number;
	int tile_width = width / col_number;

	// boost::thread_group threads;
	boost::threadpool::pool pool(4);
    for (int i=0; i < height; i += tile_height)
		for (int j = 0; j < width; j += tile_width){
			pool.schedule(boost::bind(&Scene::renderTile, this, i, j,
			                   i + tile_height < height ? i + tile_height : height,
		                       j + tile_width  < width  ? j + tile_width  : width));
		}
	pool.wait(0);
    //threads.join_all();
	/*
	boost::thread t1(&Scene::renderTile, this, 0,         0,          height/2,   width/2);
	boost::thread t2(&Scene::renderTile, this, 0,         width/2,    height/2,   width);
	boost::thread t3(&Scene::renderTile, this, height/2,  0,          height,     width/2);
	boost::thread t4(&Scene::renderTile, this, height/2,  width/2,    height,     width);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	*/
}

void Scene::renderTile(int z_start, int x_start, int z_end, int x_end) const
{
	Color color;
	Ray ray;


	// ray.o = Point3(0, -20, 1);
	ray.o = origin;

	srand(10000);

	for(int z = z_start; z < z_end; z++)
		for(int x = x_start; x < x_end; x++){
			color = traceRay(ray, z, x);
			output->addColor(color, x, z);
		}
	output->writePic();
#ifdef WITH_SAMPLE_NUMBER
	// update with anti-aliasing
	for(int z = z_start; z < z_end; z++)
		for(int x = x_start; x < x_end; x++){
			color = traceRay(ray, z, x, true);
			output->addColor(color, x, z);
		}
	output->writePic();
#endif
}

Color Scene::traceRay(Ray ray, int z, int x, bool sample) const
{
	Color color;
	int number;
#ifdef WITH_SAMPLE_NUMBER
	number = WITH_SAMPLE_NUMBER;
#else
	number = 1;
#endif
	if (sample) {
	    color = Color(0, 0, 0);
	    for(int q = 0; q < number; q++) {
	        double xx = (2 * ((x + rand_float()) * inv_w) - 1) * angle * aspectratio;
	        double zz = (1 - 2 * ((z + rand_float()) * inv_h)) * angle;
	        // ray.d = Vector3(ray.o, Point3(x, yw, z));
	        ray.d = Vector3(Point3(), Point3(xx, 4, zz));
	        ray.normalize();
	        // color += tracer_ptr->trace_ray(ray, 0);
	        color += tracer_ptr->trace_ray(ray, 1);
	    }
	    color.divide(number);
	} else {
	    double xx = (2 * ((x + 0.5) * inv_w) - 1) * angle * aspectratio;
	    double zz = (1 - 2 * ((z + 0.5) * inv_h)) * angle;
	    // ray.d = Vector3(ray.o, Point3(xx, 1, zz));
	    ray.d = Vector3(Point3(), Point3(xx, 4, zz));
	    ray.normalize();
	    color = tracer_ptr->trace_ray(ray, 1);
	}
	return color;
}
