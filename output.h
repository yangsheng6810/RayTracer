#ifndef OUTPUT_H
#define OUTPUT_H
#include "color.h"
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/multi_array.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
// #include <boost/gil/gil_all.hpp>
// #include <boost/gil/extension/io/png_dynamic_io.hpp>

typedef boost::multi_array<Color, 2> color_array_type;
typedef boost::multi_array<int, 2> number_array_type;

class Output
{
public:
	Output(int ww = 800, int hh = 600);
	~Output();
	void addColor(Color c, int x, int y);
	void writePic() const;
	void setResolution(int width_, int height_);
	int width, height;

private:
	boost::shared_ptr<color_array_type> image;
	boost::shared_ptr<number_array_type> sample;
	mutable boost::mutex mutex_output;
	mutable boost::mutex mutex_modify;
	// boost::gil::rgb8_image_t img;
	// boost::gil::rgb8_view_t v;

};

#endif // OUTPUT_H
