#ifndef OUTPUT_H
#define OUTPUT_H
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include "color.h"
#include <vector>
#include <boost/function.hpp>
#include <boost/multi_array.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/gil/gil_all.hpp>

typedef boost::multi_array<Color, 2> color_array_type;
typedef boost::multi_array<int, 2> number_array_type;

class Output
{
public:
    Output(int ww = 800, int hh = 600);
    ~Output();
    void addColor(Color c, int x, int y);
    void writePic() const;
    std::vector<Color> getTile(int x_start, int y_start, int width, int height) const;
    void outputTile(int x_start, int y_start, int width, int height) const;
    void setResolution(int width_, int height_);
    int width, height;

private:
    boost::scoped_ptr<color_array_type> image;
    boost::scoped_ptr<color_array_type> image_pool;
    boost::scoped_ptr<number_array_type> sample;
    mutable boost::mutex mutex_read;
    mutable boost::mutex mutex_write;
    boost::gil::rgb8_image_t img;
    boost::gil::rgb8_view_t v;
    std::string current_file;
    std::string current_d;

};

#endif // OUTPUT_H
