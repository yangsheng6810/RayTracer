#include <iostream>
#include "point3.h"
#include "vector3.h"

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
	Vector3 vec3 = vec1 * vec2;
	Vector3 vec4 = vec1.tensor(vec2);

	cout << p1;
	cout << p2;
	cout << vec1;
	cout << vec2;
	cout << vec3;
	cout << vec4;
	vec4 = vec3;
	cout << vec4;
}
