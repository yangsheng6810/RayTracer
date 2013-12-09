#include <iostream>
#include <boost/smart_ptr.hpp>
#include "point3.h"
#include "vector3.h"
#include "ray.h"
#include "scene.h"
#include "tracer.h"

using namespace std;

void printSuccess()
{
	cout<< "Success!"<<endl;
	exit(0);
}


int main()
{
	boost::shared_ptr<Scene> scene(new Scene(1366, 768));
	scene->setCallback(printSuccess);
	scene->setResolution(800, 600);
	scene->renderScene();
	// scene->renderScene();
	// testVector();
	cout << "Hello RayTracer!" << endl;
	// sleep(10);
	// scene->stopAllThreads();
	sleep(1000);
	return 0;
}

/**
 * @brief testVector
 * used to test vector3 and point3
 */
void testVector()
{
	Point3 p1(0, 0, 0), p2(1, 4, 2);
	Vector3 vec1(p1, p2);
	Vector3 vec2(1, 1.7, 1);
	Ray r(p2, vec1);

	cout << p1.toString() << endl;
	cout << p2.toString() << endl;
	cout << vec1.toString() << endl;
	cout << vec2.toString() << endl;
	cout << r.toString() << endl;
}
