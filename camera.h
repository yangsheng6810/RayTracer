#ifndef CAMERA_H
#define CAMERA_H
#include "point3.h"
#include "vector3.h"
#include "ray.h"

class Camera
{
public:
    Camera(Point3 origin_, int width_, int height_);
    void setVectors(Vector3 v1_, Vector3 v2_, Vector3 v3_, Vector3 v4);
    void generateVectors(Vector3 direction);
    Ray getRay(int x, int y) const;
    void setResolution(int width_, int height_);

    Point3 origin;
    int width, height;
    double inv_w, inv_h;
    Vector3 v1, v2, v3, v4;
};

#endif // CAMERA_H
