#ifndef BBOX_H
#define BBOX_H
#include "float.h"
#include "ray.h"

class BBox
{
public:
	BBox();
	BBox(const Point3 &min_, const Point3 &max_);

	Point3 min, max;
	//Returns the entry and exit distances of the ray with the
    //	bounding box.
    //If the first returned distance > the second, than
    //	the ray does not intersect the bounding box at all
    bool intersect(const Ray& r, float& t0, float& t1) const;

	// Extending the bbox with a point
	void extends(const Point3& p);

	// Extending the bbox with a bbox
	void extends(const BBox& bbox);

	//It splits the current bounding box. The center of the split and
    //the axis are needed. The left side of the current bounding box remains
    //and the right side is returned.
    BBox split(const int& _dim, const float& _splitVal);

	//It gets the current area of the surface.
    float getSurfaceArea(void);

	//It gets the average area of the three faces of the box.
    float getArea(void);

	//Returns an "empty" bounding box. Extending such a bounding
    //	box with a point will always create a bbox around the point
    //	and with a bbox - will simply copy the bbox.
    static BBox empty()
    {
        BBox ret;
	    ret.min = Point3(FLT_MAX, FLT_MAX, FLT_MAX);
	    ret.max = Point3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    }
	const Vector3 diagonal() const;
	std::string toString() const;
};

#endif // BBOX_H
