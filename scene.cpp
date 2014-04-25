#include "scene.h"
#include "threadpool.h"
#include "tracer.h"
#include "material.h"
#include "rectangle.h"
#include "parallel.h"
#include "pointlight.h"
#include "spotlight.h"
#include "rectlight.h"
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <math.h>
#include <time.h>

using namespace boost;

boost::thread waitingThread;

int count = 0;

Scene::Scene(int w, int h)
    :
    width(w),
    height(h),
    sample(1),
    sendTile(NULL),
	output(new Output(w, h)),
	tracer_ptr(new Tracer()),
	pool(new ThreadPool(4))
{
	// temporarily!
	// sample = 100;
	sample = 200;
	// buildScene(true);
	inv_w = 1.0/width;
	inv_h = 1.0/height;
	fov = 30;
	aspectratio = (double) width / height;
	angle = tan(M_PI * 0.5 * fov / (double)180);
	// pool = boost::threadpool::pool(4);
	running = false;
	row_number = 6;
	col_number = 8;
	new_object = boost::shared_ptr<TriangleMesh>();
}

Scene::~Scene()
{
	std::cout<<"in destructor"<<std::endl;
	for (int i = 0; i < objects.size(); ++i)
		objects[i].reset();
	objects.clear();
	for (int i = 0; i < lights.size(); ++i)
		lights[i].reset();
	lights.clear();
}

inline float rand_float()
{
	return ((float)rand()/ (float)RAND_MAX);
}

void Scene::buildScene(bool withBlender)
{
	/*
	shared_ptr<Material> m_sphere1(
	            new Material(
	                Color(255.0/255, 226.0/255, 190.0/255),
	                0.4,
	                Color(0.8),
    	            0.2,
    	            100,
    	            true,
    	            false));
	shared_ptr<Material> m_sphere2(
	            new Material(
	                Color(178.0/255, 255.0/255, 77.0/255),
	                0.6,
	                Color(0.8),
	                0.4,
	                100,
	                true,
	                false));
	shared_ptr<Material> m_sphere3(
	            new Material(
	                Color(0.8),
	                0.3,
	                Color(0.8),
	                0.5,
	                1000,
	                true,
	                true));
	shared_ptr<Material> m_mesh(
	            new Material(
	                Color(255.0/255, 87.0/255, 87.0/255),
	                0.6,
	                Color(0.8),
	                0.2,
	                100,
	                false,
	                false));
	shared_ptr<Material> m_ground(
	            new Material(
	                Color(0.8),
	                0.4,
	                Color(0.2),// should be 0.8
	                0.2,
	                100,
	                true,
	                false));
	shared_ptr<Material> m_background(
	            new Material(
	                Color(0.4),
	                0.4,
	                Color(0.2),
	                0.3,
	                100,
	                false,
	                false));
	shared_ptr<Material> m_rect(
	            new Material(
	                Color(0.8),
	                0.9,
    	            Color(0.9),
    	            0.1,
    	            100,
    	            true,
    	            false));

	shared_ptr<Material> m_rect2(
	            new Material(
	                Color(0.8),
	                0.9,
	                Color(0.9),
    	            0.1,
    	            100,
    	            false,
    	            false));
	objects.clear();
	if (!withBlender){
	shared_ptr<Sphere> s;
	for (int i = -2; i <= 2; i++ )
		for (int j = 0; j <= 4; j++){
	        s = shared_ptr<Sphere>(new Sphere(Point3((double)i/2.0, -1, (double)j/2.0 + 0.2), 0.2, m_sphere1));
	        objects.push_back(shared_ptr<BaseObject>(s));
		}

	s = shared_ptr<Sphere>(new Sphere(Point3(0, -2, 0.4), 0.4, m_sphere2));
	objects.push_back(shared_ptr<BaseObject>(s));

	s = shared_ptr<Sphere>(new Sphere(Point3(-0.5, -3, 0.6), 0.6, m_sphere3));
	objects.push_back(shared_ptr<BaseObject>(s));
	}

	// shared_ptr<TriangleMesh> tt = shared_ptr<TriangleMesh>(new TriangleMesh(m_mesh, false));
	// tt->addVertice(Point3( 0,  0, 0), Vector3( 0,  0, -1));
	// tt->addVertice(Point3( 0,  0, 4), Vector3( 0,  0,  1));
	// tt->addVertice(Point3( 2,  0, 2), Vector3( 1,  0,  0));
	// tt->addVertice(Point3(-2,  0, 2), Vector3(-1,  0,  0));
	// tt->addVertice(Point3( 0,  2, 2), Vector3( 0,  1,  0));
	// tt->addVertice(Point3( 0, -2, 2), Vector3( 0, -1,  0));
	// tt->addFace(0, 2, 4);
	// tt->addFace(0, 4, 3);
	// tt->addFace(0, 3, 5);
	// tt->addFace(0, 5, 2);
	// tt->addFace(1, 4, 2);
	// tt->addFace(1, 2, 5);
	// tt->addFace(1, 5, 3);
	// tt->addFace(1, 3, 4);
	// tt->finishObject();
	// std::cout<<tt->toString()<<std::endl;

	// tt->shift(Vector3(1, -4, 1));
	// objects.push_back(shared_ptr<BaseObject>(tt));

	if (!withBlender){
	shared_ptr<Plane> p;
	p = shared_ptr<Plane>(new Plane(Point3(0, 0, 0), Vector3(0, 0, 1), m_ground));// ground
	p->isGrid = false;
	objects.push_back(shared_ptr<BaseObject>(p));

	p = shared_ptr<Plane>(new Plane(Point3(0, 1, 0), Vector3(0, -1, 0), m_background));// background
	// p->isGrid = false;
	objects.push_back(shared_ptr<BaseObject>(p)); // background
	}

	// shared_ptr<Rectangle> rr =
	//         shared_ptr<Rectangle>(new Rectangle(Point3(-0.5, 1, 0), Vector3(-1.6, -6, 0), Vector3(0, 0, 3), m_rect));
	// objects.push_back(shared_ptr<BaseObject>(rr));
	
	shared_ptr<Rectangle> rl =
	        shared_ptr<Rectangle>(new Rectangle(Point3(-10, 0, 1), Vector3(0, -6, 0), Vector3(0, 0, 6), m_rect));
	objects.push_back(shared_ptr<BaseObject>(rl));
	//
	shared_ptr<Rectangle> rr =
	        shared_ptr<Rectangle>(new Rectangle(Point3(12, 0, 1), Vector3(0, -6, 0), Vector3(0, 0, 12), m_rect));
	objects.push_back(shared_ptr<BaseObject>(rr));


	lights.clear();
	shared_ptr<Light> l;

	Vector3 light_vec = Vector3(1, -2, 1);
	light_vec.normalize();
	// l = shared_ptr<Light>(new Parallel(light_vec, 5, Color(1.0)));
	// l = shared_ptr<Light>(new PointLight(Point3(-12, -9, 9), 220, Color(1.0)));
	// lights.push_back(l);
	l = shared_ptr<Light>(new RectLight(rl, 400, Color(1.0)));
	// lights.push_back(l);

	l = shared_ptr<Light>(new RectLight(rr, 160, Color(1.0)));
	// lights.push_back(l);
	*/
}

void Scene::addCamera(Point3 location, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4)
{
    camera.reset(new Camera(location, width, height));
    camera->setVectors(v1, v2, v3, v4);
}

void Scene::renderScene()
{
	if (!camera){
	    camera.reset(new Camera(Point3(0, -12, 1), width, height));
	    camera->generateVectors(Vector3(0, 6, 0));
	}
	tracer_ptr->setScene(shared_from_this());
	int tile_height = height / row_number;
	int tile_width = width / col_number;

	running = true;
	count = 0;
	int ccount = 0;

	/*
	if (!sendTile)
		buildScene(false);
		*/

	// sample
	for(int sample_i = 0; sample_i != sample; sample_i++)
	for (int i=0; i < height; i += tile_height)
		for (int j = 0; j < width; j += tile_width){
			/*
			std::cout<<ccount<<std::endl;
            ccount++;
			*/
			/*
			pool.schedule(boost::bind(&Scene::renderTile, this, i, j,
			                   i + tile_height < height ? i + tile_height : height,
		                       j + tile_width  < width  ? j + tile_width  : width));
							   */
			pool->enqueue(boost::bind(&Scene::renderTile, this, i, j,
			                   i + tile_height < height ? i + tile_height : height,
		                       j + tile_width  < width  ? j + tile_width  : width, sample_i));
    }
	// only for debug!!
	/*
	pool.schedule(boost::bind(&Scene::renderTile, this, 0, 0,
	                   tile_height < height ? tile_height : height,
	                   tile_width  < width  ? tile_width  : width,
	                   false));
   */
	// only for try!
	// if (!sendTile)
	waitingThread = boost::thread(&Scene::waiting, this);
    //threads.join_all();
}

void Scene::setCallback(boost::function<void()> f)
{
	callback = f;
}

void clearThread()
{
	std::cout<<"in at_thread_exit"<<std::endl;
	waitingThread.join();
}

void Scene::stopAllThreads()
{
	pool->clear();
	if (true){
	    boost::mutex::scoped_lock lock(mutex_running);
	    running = false;
	}
	pool->wait(0);
	std::cout<<"All threads stopped"<<std::endl;
}

void Scene::waiting()
{
    // boost::this_thread::at_thread_exit(boost::bind(clearThread));
	pool->wait(0);
	callback();
}

void Scene::setResolution(int width_, int height_)
{
	width = width_;
	height = height_;
	inv_w = 1.0 / width;
	inv_h = 1.0 / height;
	if (camera)
    	camera->setResolution(width, height);
	if (output)
		output->setResolution(width, height);
}

bool isBlack(const Color& c)
{
	return fabs(c.r) + fabs(c.g) + fabs(c.b) < 0.01;
}

double difference(const Color& c1, const Color& c2)
{
	return fabs(c1.r - c2.r)
	        + fabs(c1.g - c2.g)
	        + fabs(c1.b - c2.b);
}

void Scene::renderTile(int z_start, int x_start, int z_end, int x_end, int sample_n) const
{
	Color color;
	Ray ray;
	int index = 0;
	/*
	std::vector<Color> tileColor = output->getTile(x_start,
	                                               z_start,
	                                               x_end - x_start,
	                                               z_end - z_start);
												   */

	try{
		for(int z = z_start; z < z_end; z++){
			for(int x = x_start; x < x_end; x++){
				// color = Color(0.5);
				color = Color(0, 0, 0);
				ray = camera->getRay(x, z);
				ray.normalize();

			    color += tracer_ptr->trace_ray(ray, 1, sample_n);
				output->addColor(color, x, z);
			}
	    }
		if (!running)
		    return;
		output->writePic();
	    output->outputTile(x_start, z_start, x_end - x_start, z_end - z_start);
		// std::cout<<"before calling sendTile"<<std::endl;
		if (sendTile){
		    if (!running)
		        return;
			// std::cout<<"in if clause"<<std::endl;
            boost::mutex::scoped_lock lock(mutex_update);
			// std::cout<<"acquired lock"<<std::endl;
            sendTile(x_start, z_start, x_end - x_start, z_end - z_start);
			// std::cout<<"after calling sendTile"<<std::endl;
		} else {
			// come here when not with blender
			// std::cout<<"sendTile not prepared!"<<std::endl;
		}
	}
	catch(boost::thread_interrupted& )
	{
		std::cout<<"interrupted"<<std::endl;
	}
}

void Scene::setSample(int sample_)
{
	sample = sample_;
}

int Scene::getThreadNum()
{
	return sample * row_number * col_number;
}

void Scene::setGrid(int row_number_, int col_number_)
{
	row_number = row_number_;
	col_number = col_number_;
}

void Scene::setSendTile(boost::function<void (int, int, int, int)> f)
{
    sendTile = f;
}

void Scene::addObject(bool isSmooth)
{
	new_object = boost::shared_ptr<TriangleMesh>(new TriangleMesh(isSmooth));
}

void Scene::addMaterial(const shared_ptr<Material>& m)
{
	new_object->addMaterial(m);
}

void Scene::addVertice(const Point3& point, const Vector3& normal)
{
	new_object->addVertice(point, normal);
	// std::cout<<point.toString()<<std::endl;
}

void Scene::addFace(int v1, int v2, int v3, size_t material_index)
{
	new_object->addFace(v1, v2, v3, material_index);
	// std::cout<<v1<<", "<<v2<<", "<<v3<<std::endl;
}

void Scene::finishObject()
{
	new_object->finishObject();
	std::cout<<"in finishObject"<<std::endl;
	objects.push_back(new_object);
	new_object.reset();
}


void Scene::addLamp(Point3 location, Vector3 direction, Color color, double energy, double spot_size)
{
	std::cout<<"come to scene::addLamp"<<std::endl;
    lights.push_back(shared_ptr<Light>(
	                     new SpotLight(location, direction, color, energy, spot_size)));
}
