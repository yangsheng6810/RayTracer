#ifndef OUTPUT_H
#define OUTPUT_H
#include "color.h"
#include <boost/multi_array.hpp>
#include <boost/smart_ptr.hpp>

typedef boost::multi_array<Color, 2> array_type;
typedef array_type::index index;

class Output
{
public:
	Output(int ww = 800, int hh = 600);
	~Output();
	void addColor(Color c, int x, int y);
	void writePic();
	int width, height;
	boost::scoped_ptr<array_type> image;
};

#endif // OUTPUT_H
