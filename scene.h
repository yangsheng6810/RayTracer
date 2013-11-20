#ifndef SCENE_H
#define SCENE_H
#include "baseobject.h"
#include "sphere.h"
#include "plane.h"
#include "output.h"
#include "light.h"
#include "camera.h"
#include <vector>
#include "boost/threadpool.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>

class Tracer;

class Scene:
        public boost::enable_shared_from_this<Scene>
{
public:
	Scene(int w = 640, int h = 480);
    void createTracer();
	void buildScene();
	void buildScene1();
    void renderScene();
    void addCamera(Point3 location, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);


    void renderSceneThread();
    void renderSceneThreadCamera();
    void renderTile(int z_start, int x_start, int z_end, int x_end, bool sample) const;
    void renderTile_no_samp(int z_start, int x_start, int z_end, int x_end) const;
    void renderTile_samp(int z_start, int x_start, int z_end, int x_end) const;
    void renderTile_samp_camera(int z_start, int x_start, int z_end, int x_end) const;
	Color traceRay_no_samp(Ray ray, int z, int x) const;
	Color traceRay_samp(Ray ray, int z, int x) const;
	bool hitBack(const Ray& ray, double& tmin, ShadePacket& sp) const;
	bool hitSphere(const Ray& ray, double& tmin, ShadePacket& sp) const;
	bool hitFloor(const Ray& ray, double& tmin, ShadePacket& sp) const;

    void setCallback(boost::function<void()> f);
	void waiting();
	void stopAllThreads();

	void setResolution(int width_, int height_);

	int width, height;
	double inv_w, inv_h;
	double fov, aspectratio, angle;
	Point3 origin;
	boost::shared_ptr<Camera> camera;

	std::vector<boost::shared_ptr<BaseObject> > objects;
	std::vector<boost::shared_ptr<Light> > lights;
	/*
	boost::scoped_ptr<Sphere> sphere;
	boost::scoped_ptr<Plane> floor;
	boost::scoped_ptr<Plane> background;
	*/

	boost::scoped_ptr<Output> output;
	boost::scoped_ptr<Tracer> tracer_ptr;
	boost::threadpool::pool pool;
	boost::thread waitingThread;
	boost::function<void()> callback;
};

#endif // SCENE_H
