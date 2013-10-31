#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "baseobject.h"
#include "sphere.h"
#include "plane.h"
#include "output.h"

class Tracer;

class Scene:
        public boost::enable_shared_from_this<Scene>
{
public:
	Scene(int w = 640, int h = 480);
    void createTracer();
	void buildScene();
    void renderScene();


    void renderSceneThread();
    void renderTile(int z_start, int x_start, int z_end, int x_end) const;
	Color traceRay(Ray ray, int z, int x, bool sample = false) const;
	bool hitBack(const Ray& ray, double& tmin, ShadePacket& sp) const;
	bool hitSphere(const Ray& ray, double& tmin, ShadePacket& sp) const;
	bool hitFloor(const Ray& ray, double& tmin, ShadePacket& sp) const;

	int width, height;
	double inv_w, inv_h;
	double fov, aspectratio, angle;
	Point3 origin;

	std::vector<boost::shared_ptr<BaseObject> > objects;
	/*
	boost::scoped_ptr<Sphere> sphere;
	boost::scoped_ptr<Plane> floor;
	boost::scoped_ptr<Plane> background;
	*/

	boost::scoped_ptr<Output> output;
	boost::scoped_ptr<Tracer> tracer_ptr;
};

#endif // SCENE_H
