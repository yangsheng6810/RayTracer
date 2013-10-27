#include <iostream>
#include "point3.h"
#include "vector3.h"
#include "ray.h"

using namespace std;

void testVector();

int main()
{
	testVector();
	cout << "Hello RayTracer!" << endl;
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
