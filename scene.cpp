#include "scene.h"
#include "tracer.h"
#include "material.h"
#include "trianglemesh.h"
#include "rectangle.h"
#include "parallel.h"
#include "pointlight.h"
#include "rectlight.h"
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <math.h>
#include <time.h>

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
	pool = boost::threadpool::pool(4);
}

inline float rand_float()
{
	return ((float)rand()/ (float)RAND_MAX);
}

void Scene::buildScene()
{
	shared_ptr<Material> m_sphere1(
	            new Material(
	                Color(92.0/255, 228.0/255, 255.0/255),
	                0.6,
    	            Color(0.2),
    	            0.2,
    	            Color(0.4),
    	            Color(0.3),
    	            100,
    	            true,
    	            true));
	shared_ptr<Material> m_sphere2(
	            new Material(
	                Color(178.0/255, 255.0/255, 77.0/255),
	                // Color(0.2),
	                0.6,
	                Color(0.2),
	                0.2,
	                Color(0.3),
    	            Color(0.3),
	                100,
	                true,
	                false));
	shared_ptr<Material> m_sphere3(
	            new Material(
	                Color(0.8),
	                // Color(0.2),
	                0.1,
	                Color(0.8),
	                0.9,
	                Color(1),
    	            Color(0.3),
	                100,
	                true,
	                true));
	shared_ptr<Material> m_mesh(
	            new Material(
	                Color(255.0/255, 87.0/255, 87.0/255),
	                0.1,
	                Color(0.5),
	                0.9,
	                Color(0.1),
    	            Color(0.3),
	                100,
	                true,
	                false));
	shared_ptr<Material> m_ground(
	            new Material(
	                Color(0.8),
	                0.4,
	                Color(0.2),// should be 0.8
	                0.2,
	                Color(0.1),
    	            Color(0.3),
	                100,
	                true,
	                false));
	shared_ptr<Material> m_background(
	            new Material(
	                Color(0.7),
	                0.4,
	                Color(0.7),
	                0.1,
	                Color(0.2),
    	            Color(0.3),
	                100,
	                true,
	                false));
	shared_ptr<Material> m_rect(
	            new Material(
	                Color(0.8),
	                0.9,
    	            Color(0.4),
    	            0.1,
    	            Color(0.2),
    	            Color(0),
    	            100,
    	            true,
    	            false));

	objects.clear();
	shared_ptr<Sphere> s;
	for (int i = -2; i <= 2; i++ )
		for (int j = 0; j <= 4; j++){
	        s = shared_ptr<Sphere>(new Sphere(Point3((double)i/2.0, 3, (double)j/2.0 + 0.2), 0.2, m_sphere1));
	        objects.push_back(shared_ptr<BaseObject>(s));
		}

	s = shared_ptr<Sphere>(new Sphere(Point3(0, -2, 0.4), 0.4, m_sphere2));
	objects.push_back(shared_ptr<BaseObject>(s));

	s = shared_ptr<Sphere>(new Sphere(Point3(-0.5, -4, 0.6), 0.6, m_sphere3));
	objects.push_back(shared_ptr<BaseObject>(s));

	shared_ptr<TriangleMesh> tt = shared_ptr<TriangleMesh>(new TriangleMesh(m_mesh));
	tt->addVertice(Point3( 0,  0, 0), Vector3( 0,  0, -1));
	tt->addVertice(Point3( 0,  0, 1), Vector3( 0,  0,  1));
	tt->addVertice(Point3( 0.5,  0, 0.5), Vector3( 1,  0,  0));
	tt->addVertice(Point3(-0.5,  0, 0.5), Vector3(-1,  0,  0));
	tt->addVertice(Point3( 0,  0.5, 0.5), Vector3( 0,  1,  0));
	tt->addVertice(Point3( 0, -0.5, 0.5), Vector3( 0, -1,  0));
	tt->addFace(0, 2, 4);
	tt->addFace(0, 4, 3);
	tt->addFace(0, 3, 5);
	tt->addFace(0, 5, 2);
	tt->addFace(1, 4, 2);
	tt->addFace(1, 2, 5);
	tt->addFace(1, 5, 3);
	tt->addFace(1, 3, 4);
	// std::cout<<tt->toString()<<std::endl;

	tt->shift(Vector3(1, -1, 1));
	objects.push_back(shared_ptr<BaseObject>(tt));

	shared_ptr<Plane> p;
	p = shared_ptr<Plane>(new Plane(Point3(0, 0, 0), Vector3(0, 0, 1), m_ground));// ground
	p->isGrid = false;
	objects.push_back(shared_ptr<BaseObject>(p));

	p = shared_ptr<Plane>(new Plane(Point3(0, 6, 0), Vector3(0, -1, 0), m_background));// background
	// p->isGrid = false;
	objects.push_back(shared_ptr<BaseObject>(p)); // background

	// shared_ptr<Rectangle> rr =
	//         shared_ptr<Rectangle>(new Rectangle(Point3(-0.5, 1, 0), Vector3(-1.6, -6, 0), Vector3(0, 0, 3), m_rect));
	// objects.push_back(shared_ptr<BaseObject>(rr));
	
	shared_ptr<Rectangle> rl =
	        shared_ptr<Rectangle>(new Rectangle(Point3(-6, 0, 1), Vector3(0, -6, 0), Vector3(0, 0, 6), m_rect));
	// objects.push_back(shared_ptr<BaseObject>(rl));
	//
	shared_ptr<Rectangle> rr =
	        shared_ptr<Rectangle>(new Rectangle(Point3(6, 0, 1), Vector3(0, -6, 0), Vector3(0, 0, 6), m_rect));
	// objects.push_back(shared_ptr<BaseObject>(rr));


	lights.clear();
	shared_ptr<Light> l;

	Vector3 light_vec = Vector3(1, -2, 1);
	light_vec.normalize();
	// l = shared_ptr<Light>(new Parallel(light_vec, 5, Color(1.0)));
	// l = shared_ptr<Light>(new PointLight(Point3(-12, -9, 9), 220, Color(1.0)));
	// lights.push_back(l);
	l = shared_ptr<Light>(new RectLight(rl, 200, Color(1.0)));
	lights.push_back(l);

	l = shared_ptr<Light>(new RectLight(rr, 60, Color(1.0)));
	lights.push_back(l);
}

/**
 * @brief buildScene1
 * build the scene for rendering
 */
void Scene::buildScene1()
{
	/*
	shared_ptr<Material> m_sphere1(
	            new Material(
	                Color(92.0/255, 228.0/255, 255.0/255),
    	            Color(0.7),
    	            Color(0.4),
    	            Color(0.1),
    	            100,
    	            true,
    	            true));
	shared_ptr<Material> m_sphere2(
	            new Material(
	                Color(178.0/255, 255.0/255, 77.0/255),
	                // Color(0.2),
	                Color(0.6),
	                Color(0.4),
    	            Color(0.1),
	                100,
	                true,
	                true));
	shared_ptr<Material> m_mesh(
	            new Material(
	                Color(255.0/255, 87.0/255, 87.0/255),
	                Color(0.7),
	                Color(0.4),
    	            Color(0.1),
	                100,
	                true,
	                true));
	shared_ptr<Material> m_ground(
	            new Material(
	                Color(0.7),
	                Color(0.8),// should be 0.8
	                Color(0.2),
    	            Color(0.1),
	                100,
	                true,
	                false));
	shared_ptr<Material> m_background(
	            new Material(
	                Color(0.7),
	                Color(0.8),
	                Color(0.2),
    	            Color(0.1),
	                100,
	                true,
	                false));
	shared_ptr<Material> m_rect(
	            new Material(
	                Color(0.8),
    	            Color(0.8),
    	            Color(0.4),
    	            Color(0),
    	            100,
    	            true,
    	            false));

	objects.clear();
	shared_ptr<Sphere> s;
	for (int i = -2; i <= 2; i++ )
		for (int j = 0; j <= 4; j++){
	        s = shared_ptr<Sphere>(new Sphere(Point3((double)i/2.0, 3, (double)j/2.0 + 0.2), 0.2, m_sphere1));
	        objects.push_back(shared_ptr<BaseObject>(s));
		}

	s = shared_ptr<Sphere>(new Sphere(Point3(0, -3, 1), 0.4, m_sphere2));
	objects.push_back(shared_ptr<BaseObject>(s));

	shared_ptr<TriangleMesh> tt = shared_ptr<TriangleMesh>(new TriangleMesh(m_mesh));
	tt->addVertice(Point3( 0,  0, 0), Vector3( 0,  0, -1));
	tt->addVertice(Point3( 0,  0, 1), Vector3( 0,  0,  1));
	tt->addVertice(Point3( 0.5,  0, 0.5), Vector3( 1,  0,  0));
	tt->addVertice(Point3(-0.5,  0, 0.5), Vector3(-1,  0,  0));
	tt->addVertice(Point3( 0,  0.5, 0.5), Vector3( 0,  1,  0));
	tt->addVertice(Point3( 0, -0.5, 0.5), Vector3( 0, -1,  0));
	tt->addFace(0, 2, 4);
	tt->addFace(0, 4, 3);
	tt->addFace(0, 3, 5);
	tt->addFace(0, 5, 2);
	tt->addFace(1, 4, 2);
	tt->addFace(1, 2, 5);
	tt->addFace(1, 5, 3);
	tt->addFace(1, 3, 4);
	// std::cout<<tt->toString()<<std::endl;

	tt->shift(Vector3(1, -1, 1));
	objects.push_back(shared_ptr<BaseObject>(tt));

	shared_ptr<Plane> p;
	p = shared_ptr<Plane>(new Plane(Point3(0, 0, 0), Vector3(0, 0, 1), m_ground));// ground
	// p->isGrid = false;
	objects.push_back(shared_ptr<BaseObject>(p));

	// p = shared_ptr<Plane>(new Plane(Point3(0, 6, 0), Vector3(0, -1, 0), m_background));// background
	// p->isGrid = false;
	// objects.push_back(shared_ptr<BaseObject>(p)); // background

	// shared_ptr<Rectangle> rr =
	//         shared_ptr<Rectangle>(new Rectangle(Point3(-0.5, 1, 0), Vector3(-1.6, -6, 0), Vector3(0, 0, 3), m_rect));
	// objects.push_back(shared_ptr<BaseObject>(rr));
	
	shared_ptr<Rectangle> rl =
	        shared_ptr<Rectangle>(new Rectangle(Point3(-2, 4, 0), Vector3(0, -2, 0), Vector3(0, 0, 3), m_rect));
	objects.push_back(shared_ptr<BaseObject>(rl));

	lights.clear();
	shared_ptr<Light> l;

	Vector3 light_vec = Vector3(1, -1, 1);
	light_vec.normalize();
	l = shared_ptr<Light>(new Parallel(light_vec, 1, Color(1.0)));
	// shared_ptr<Light> l = shared_ptr<Light>(new PointLight(Point3(10, -2, 10), 1.4, Color(1.0)));
	lights.push_back(l);
	l = shared_ptr<Light>(new RectLight(rl, 1.4, Color(1.0)));
	lights.push_back(l);
	*/

}

void Scene::renderScene()
{
	Color color;
	Ray ray;

	tracer_ptr->setScene(shared_from_this());

	ray.o = Point3(0, -10, 0.4);

	srand(time(NULL));

	for(int z = 0; z < height; z++)
		for(int x = 0; x < width; x++){
			color = traceRay_no_samp(ray, z, x);
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
	origin = Point3(0, -15, 0.4);
	tracer_ptr->setScene(shared_from_this());
	int row_number = 6;
	int col_number = 8;
	int tile_height = height / row_number;
	int tile_width = width / col_number;

	// boost::thread_group threads;
	// no_sample
	/*
    for (int i=0; i < height; i += tile_height)
		for (int j = 0; j < width; j += tile_width){
			pool.schedule(boost::bind(&Scene::renderTile, this, i, j,
			                   i + tile_height < height ? i + tile_height : height,
		                       j + tile_width  < width  ? j + tile_width  : width, false));
		}
	*/

	// sample
	for(int sample = 0; sample != 10; sample++)
	for (int i=0; i < height; i += tile_height)
		for (int j = 0; j < width; j += tile_width){
			pool.schedule(boost::bind(&Scene::renderTile, this, i, j,
			                   i + tile_height < height ? i + tile_height : height,
		                       j + tile_width  < width  ? j + tile_width  : width, true));
		}
	// only for test!!
	/*
	for(int sample = 0; sample != 10; sample++)
	for (int i=0; i < height; i += tile_height)
		for (int j = 0; j < width; j += tile_width){
			if (j == tile_width &&
					(i == tile_height || i == tile_height * 2 ||
					 i == tile_height * 3 || i == tile_height * 4))
			pool.schedule(boost::bind(&Scene::renderTile, this, i, j,
			                   i + tile_height < height ? i + tile_height : height,
		                       j + tile_width  < width  ? j + tile_width  : width, true));
		}
	*/
	waitingThread = boost::thread(&Scene::waiting, this);
    //threads.join_all();
}

void Scene::setCallback(boost::function<void()> f)
{
	callback = f;
}

void Scene::waiting()
{
	pool.wait(0);
	callback();
}

void Scene::renderTile(int z_start, int x_start, int z_end, int x_end, bool sample) const
{
	if (sample)
		renderTile_samp(z_start, x_start, z_end, x_end);
	else
		renderTile_no_samp(z_start, x_start, z_end, x_end);
}

void Scene::renderTile_no_samp(int z_start, int x_start, int z_end, int x_end) const
{
	Color color;
	Ray ray;

	// ray.o = Point3(0, -20, 1);
	ray.o = origin;

	srand(10000);

	for(int z = z_start; z < z_end; z++)
		for(int x = x_start; x < x_end; x++){
			color = traceRay_no_samp(ray, z, x);
			output->addColor(color, x, z);
		}
	output->writePic();
}

void Scene::renderTile_samp(int z_start, int x_start, int z_end, int x_end) const
{
	Color color;
	Ray ray;

	// ray.o = Point3(0, -20, 1);
	ray.o = origin;

	srand(10000);

	for(int kkk = 0; kkk != 1; kkk++){
		for(int z = z_start; z < z_end; z++){
			for(int x = x_start; x < x_end; x++){
				color = traceRay_samp(ray, z, x);
				output->addColor(color, x, z);
			}
		}
		output->writePic();
	}

}
Color Scene::traceRay_no_samp(Ray ray, int z, int x) const
{
	Color color;
	double xx = (2 * ((x + 0.5) * inv_w) - 1) * angle * aspectratio;
	double zz = (1 - 2 * ((z + 0.5) * inv_h)) * angle;
	// ray.d = Vector3(ray.o, Point3(xx, 1, zz));
	// ray.d = Vector3(Point3(), Point3(xx, 4, zz));
	ray.d = Vector3(Point3(), Point3(xx, 3, zz));
	ray.normalize();
	return tracer_ptr->trace_ray(ray, 1);
}

Color Scene::traceRay_samp(Ray ray, int z, int x) const
{
	Color color;
	// int number = 20;
	int number = 1;
	color = Color(0, 0, 0);
	for(int q = 0; q < number; q++) {
	    double xx = (2 * ((x + rand_float()) * inv_w) - 1) * angle * aspectratio;
	    double zz = (1 - 2 * ((z + rand_float()) * inv_h)) * angle;
	    // ray.d = Vector3(ray.o, Point3(x, yw, z));
	    // ray.d = Vector3(Point3(), Point3(xx, 4, zz));
	    ray.d = Vector3(Point3(), Point3(xx, 3, zz));
	    ray.normalize();
	    // color += tracer_ptr->trace_ray(ray, 0);
	    color += tracer_ptr->trace_ray(ray, 1);
	}
	color.divide(number);
	return color;
}
