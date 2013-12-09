#include <iostream>
#include <string>
#include "scene.h"
#include "tracer.h"
#include "vector3.h"
#include "point3.h"
#include "color.h"
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/python.hpp>
using namespace boost::python;

boost::shared_ptr<Scene> scene;
PyObject *renderEngine;
PyObject *pythonCallback;
PyObject *pythonAddTile;
int total_sample = 0;

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

void add_object()
{
	scene->addObject();
}

void add_vertice(object point, object normal)
{
	Point3 c_point = Point3(extract<double>(point.attr("x")),
	                        extract<double>(point.attr("y")),
	                        extract<double>(point.attr("z")));
	Vector3 c_normal = Vector3(extract<double>(normal.attr("x")),
	                           extract<double>(normal.attr("y")),
	                           extract<double>(normal.attr("z")));
	scene->addVertice(c_point, c_normal);
}

void add_face(object v1, object v2, object v3)
{
	scene->addFace(extract<int>(v1),
	               extract<int>(v2),
	               extract<int>(v3));
}

void finish_object()
{
	scene->finishObject();
}

std::string parse_python_exception(){
    PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
    std::string ret("Unfetchable Python error");
    if(type_ptr != NULL){
        handle<> h_type(type_ptr);
        str type_pstr(h_type);
        extract<std::string> e_type_pstr(type_pstr);
        if(e_type_pstr.check())
            ret = e_type_pstr();
        else
            ret = "Unknown exception type";
    }
    if(value_ptr != NULL){
        handle<> h_value(value_ptr);
        str value_pstr(h_value);
        extract<std::string> e_value_pstr(value_pstr);
        if (e_value_pstr.check())
            ret += e_value_pstr();
        else
            ret += "Unknown pvalue";
    }
    if(traceback_ptr != NULL){
        handle<> h_tb(traceback_ptr);
        object tb(import("traceback"));
        object fmt_tb(tb.attr("format_tb"));
        object tb_list(fmt_tb(h_tb));
        object tb_str(str("\n").join(tb_list));
        extract<std::string> returned(tb_str);
        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }
    return ret;
}



void addTile(int x_start, int y_start, int width, int height)
{
	/*
	tryClass *tt = new tryClass(x_start, y_start, width, height, data, is_final);
	delete tt;
	tt = NULL;
	*/
	// std::cout<<"Before calling back!"<<std::endl;
	PyEval_InitThreads();

    PyGILState_STATE state = PyGILState_Ensure();
    try{
        call_method<void>(renderEngine, "update_tile", x_start, y_start, width, height);
    }catch(boost::python::error_already_set const &){
        std::string perror_str = parse_python_exception();
        std::cout << "Error in Python: " << perror_str << std::endl;
    }
    PyGILState_Release(state);
	// std::cout<<"After calling back!"<<std::endl;
}

char const* new_scene(PyObject *engine)
{
	scene = boost::shared_ptr<Scene>(new Scene(1366, 678));
	renderEngine = engine;
	Py_INCREF(renderEngine);

	scene->setSendTile(addTile);
	return "Success!";
}

void callback()
{
	std::cout<<"Success!"<<std::endl;
}

char const* render_scene(void)
{
	scene->setCallback(callback);
	scene->renderScene();
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

void set_sample(object sample_)
{
	total_sample = extract<int>(sample_);
	scene->setSample(total_sample);
}

object get_thread_num()
{
	// std::cout<<"in get_thread_num()"<<std::endl;
	return object(scene->getThreadNum());
}

void set_grid(object row_n, object col_n)
{
	int row_number = extract<int>(row_n);
	int col_number = extract<int>(col_n);
	scene->setGrid(row_number, col_number);
}

BOOST_PYTHON_MODULE(librender)
{
  def("yay", yay);
  def("add_object", add_object);
  def("add_vertice", add_vertice, args("point", "normal"));
  def("add_face", add_face, args("v1", "v2", "v3"));
  def("finish_object", finish_object);
  def("get_thread_num", get_thread_num);
  def("add_objects", add_objects, args("s"), "try docstring");
  def("add_camera", add_camera, args("location", "p1", "p2", "p3", "p4"), "try docstring");
  def("new_scene",   new_scene, args("engine"));
  def("render_scene", render_scene);
  def("stop_render", stop_render);
  def("set_resolution", set_resolution, args("width_", "height_"));
  def("set_sample", set_sample, args("sample_"));
  def("set_grid", set_grid, args("row_n", "col_n"));
}
