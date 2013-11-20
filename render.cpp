#include <string>
#include "scene.h"
#include "tracer.h"
#include "vector3.h"
#include "point3.h"
#include <boost/smart_ptr.hpp>
#include <boost/python.hpp>
using namespace boost::python;

boost::shared_ptr<Scene> scene;

char const* yay()
{
  return "Yay!";
}

char const* add_objects(object s)
{
    //first we need to get the this attribute from the Python Object
    char const* sstr = extract<char const*>(s.attr("name"));
    return sstr;
}

char const* new_scene(void)
{
	scene = boost::shared_ptr<Scene>(new Scene(1366, 678));
	return "Success!";
}

void callback()
{
	std::cout<<"Success!"<<std::endl;
}

char const* render_scene(void)
{
	scene->setCallback(callback);
	scene->renderSceneThreadCamera();
	return "Running!";
}
Vector3 get_vect(object v)
{
	return Vector3(extract<double>(v.attr("x")),
	               extract<double>(v.attr("y")),
	               extract<double>(v.attr("z")));
}

Point3 get_point(object p)
{
	return Point3(extract<double>(p.attr("x")),
	              extract<double>(p.attr("y")),
	              extract<double>(p.attr("z")));
}

void add_camera(object location, object p1, object p2, object p3, object p4)
{
	Point3 loc = get_point(location),
	        point1 = get_point(p1),
	        point2 = get_point(p2),
	        point3 = get_point(p3),
	        point4 = get_point(p4);
	scene->addCamera(loc,
	                 Vector3(loc, point1),
	                 Vector3(loc, point2),
	                 Vector3(loc, point3),
	                 Vector3(loc, point4));
}

void stop_render()
{
	scene->stopAllThreads();
}

void set_resolution(object width_, object height_)
{
	int width = extract<int>(width_);
	int height = extract<int>(height_);
	scene->setResolution(width, height);
}

BOOST_PYTHON_MODULE(librender)
{
  def("yay", yay);
  def("add_objects", add_objects, args("s"), "try docstring");
  def("add_camera", add_camera, args("location", "p1", "p2", "p3", "p4"), "try docstring");
  def("new_scene",   new_scene);
  def("render_scene", render_scene);
  def("stop_render", stop_render);
  def("set_resolution", set_resolution, args("width_", "height_"));
}
