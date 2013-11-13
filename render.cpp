#include <string>
#include "scene.h"
#include "tracer.h"
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
	scene->renderSceneThread();
	return "Running!";
}

BOOST_PYTHON_MODULE(librender)
{
  def("yay", yay);
  def("add_objects", add_objects, args("s"), "try docstring");
  def("new_scene",   new_scene);
  def("render_scene", render_scene);
}
