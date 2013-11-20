#include "camera.h"
#include <cstdlib>
#include <time.h>

Camera::Camera(Point3 origin_, int width_, int height_):
    origin(origin_), width(width_), height(height_)
{
	inv_w = 1.0 / width;
	inv_h = 1.0 / height;
	srand(time(NULL));
}
inline float rand_float()
{
	return ((float)rand()/ (float)RAND_MAX);
}

void Camera::setVectors(Vector3 v1_, Vector3 v2_, Vector3 v3_, Vector3 v4_)
{
	v1 = v1_;
	v2 = v2_;
	v3 = v3_;
	v4 = v4_;
}

void Camera::generateVectors(Vector3 direction)
{
	Vector3 xx = direction.tensor(Vector3(0, 0, 1));
	xx.normalize();
	Vector3 yy = - direction.tensor(xx);
	yy.normalize();
	double ratio = height * 1.0/width;
	v1 = direction - xx + yy * ratio;
	v2 = direction + xx + yy * ratio;
	v3 = direction - xx - yy * ratio;
	v4 = direction + xx - yy * ratio;
}

Ray Camera::getRay(int x, int y) const
{
	Ray r = Ray();
	r.o = origin;
	double p1, p2;
	p1 = (x + rand_float() - 0.5) * inv_w;
	p2 = (y + rand_float() - 0.5) * inv_h;
	Vector3 x1 = (1 - p1) * v1 + p1 * v2;
	Vector3 x2 = (1 - p1) * v3 + p1 * v4;
	Vector3 yy = (1 - p2) * x1 + p2 * x2;
	r.d = yy;
	return r;
}

void Camera::setResolution(int width_, int height_)
{
	width = width_;
	height = height_;
	inv_w = 1.0 / width;
	inv_h = 1.0 / height;
}
