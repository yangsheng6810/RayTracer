#ifndef SCENE_H
#define SCENE_H
#include "baseobject.h"
#include "trianglemesh.h"
#include "sphere.h"
#include "plane.h"
#include "output.h"
#include "light.h"
#include "camera.h"
#include "constants.h"
#include <vector>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>

class Tracer;
class ThreadPool;

class Scene:
        public boost::enable_shared_from_this<Scene>
{
public:
    Scene(int w = 640, int h = 480);
    ~Scene();
    void createTracer();
    void buildScene(bool withBlender = false);
    void addCamera(Point3 location, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);

    void buildTree();
    void buildTreeForObj(int i);
    void renderScene();
    void renderTile(int z_start, int x_start, int z_end, int x_end, int sample_n) const;
    bool hitBack(const Ray& ray, double& tmin, ShadePacket& sp) const;
    bool hitSphere(const Ray& ray, double& tmin, ShadePacket& sp) const;
    bool hitFloor(const Ray& ray, double& tmin, ShadePacket& sp) const;

    void setCallback(boost::function<void ()> f);
    void setSendTile(boost::function<void (int, int, int, int)> f);
    void clearThread();
    void waiting();
    void stopAllThreads();

    void setResolution(int width_, int height_);
    void setSample(int sample_);
    void setGrid(int row_number_, int col_number_);
    int getThreadNum();

    void addObject(bool isSmooth = false);
    void addMaterial(const boost::shared_ptr<Material>& m);
    void addVertice(const Point3& point, const Vector3& normal);
    void addFace(int v1, int v2, int v3, size_t material_index);
    void finishObject();
    void addLamp(Point3 location, Vector3 direction, double distance, Color color, double energy, double spot_size);
    boost::shared_ptr<TriangleMesh> new_object;

    boost::function<void (int, int, int, int)> sendTile;
    int width, height;
    int row_number, col_number;
    int sample;
    double inv_w, inv_h;
    double fov, aspectratio, angle;
    Point3 origin;
    boost::scoped_ptr<Camera> camera;

    std::vector<boost::shared_ptr<BaseObject> > objects;
    std::vector<boost::shared_ptr<Light> > lights;

    boost::scoped_ptr<Output> output;
    boost::scoped_ptr<Tracer> tracer_ptr;
    // boost::threadpool::pool pool;
    boost::scoped_ptr<ThreadPool> pool;
    boost::scoped_ptr<ThreadPool> pool_for_tree;
    // boost::thread waitingThread;
    boost::function<void()> callback;
    mutable boost::mutex mutex_update;
    bool running;
    mutable boost::mutex mutex_running;
    mutable boost::mutex mutex_count;
};

#endif // SCENE_H
