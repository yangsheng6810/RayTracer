#include <fstream>
#define BOOST_SYSTEM_NO_LIB
#define BOOST_FILESYSTEM_NO_LIB
#include "output.h"
#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
using namespace boost::gil;

Output::Output(int ww, int hh):
    width(ww), height(hh),
	image(new color_array_type(boost::extents[height][width])),
	image_pool(new color_array_type(boost::extents[height][width])),
    sample(new number_array_type(boost::extents[height][width]))
{
    // img = rgb8_image_t(ww, hh);
    // v = view(img);
	// rgb8_pixel_t black(0, 0, 0);
    // fill_pixels(view(img), black);
	img = rgb8_image_t(width, height);
	v = boost::gil::view(img);
	Color c("black");
	for (int i = 0; i < width; ++i)
		for(int j = 0; j < height; ++j){
	        (*image)[j][i] = c;
	        (*image_pool)[j][i] = c;
	        (*sample)[j][i] = 0;
		}

	namespace fs = boost::filesystem;
	boost::filesystem::path temp_d = "/tmp/yang-blender";
	if (boost::filesystem::exists(temp_d)){
		remove_all(temp_d);
	}
	boost::filesystem::create_directory(temp_d);

    current_d = "/home/yangsheng/Blender/ppm_output";
	boost::filesystem::path animate_d = current_d;
	if (!boost::filesystem::exists(animate_d))
		boost::filesystem::create_directory(animate_d);
	bool finished = false;
	int i = 1;
	while(!finished){
		std::ostringstream strs;
		strs<<current_d<<"/"<<std::setw(4)<<std::setfill('0')<<i<<".ppm";
		animate_d = strs.str();
		if (!boost::filesystem::exists(animate_d)){
			current_file = strs.str();
			finished = true;
		}
		i++;
	}
}

Output::~Output()
{
}

void Output::setResolution(int width_, int height_)
{
	width = width_;
	height = height_;
	image.reset(new color_array_type(boost::extents[height][width]));
	image_pool.reset(new color_array_type(boost::extents[height][width]));
    sample.reset(new number_array_type(boost::extents[height][width]));
	img = rgb8_image_t(width, height);
	v = boost::gil::view(img);
}

void Output::writePic() const
{
    boost::mutex::scoped_lock lock_m(mutex_read);
	// std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
	std::ofstream ofs(current_file.c_str(), std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < height; ++i)
		for(unsigned j = 0; j < width; ++j){
			/*
    		ofs << (unsigned char)((*image)[i][j].r / (*sample)[i][j] * 255) <<
	    	(unsigned char)((*image)[i][j].g / (*sample)[i][j] * 255) <<
		    (unsigned char)((*image)[i][j].b / (*sample)[i][j] * 255);
			*/
    		ofs << (unsigned char)((*image)[i][j].r * 255) <<
	    	(unsigned char)((*image)[i][j].g * 255) <<
		    (unsigned char)((*image)[i][j].b * 255);
	    }
	ofs.close();
	/*
	for (unsigned i = 0; i < height; ++i)
		for(unsigned j = 0; j < width; ++j)
    		v(j,i) = rgb8_pixel_t(
			             (unsigned char)((*image)[i][j].r / (*sample)[i][j] * 255),
			             (unsigned char)((*image)[i][j].g / (*sample)[i][j] * 255),
            		     (unsigned char)((*image)[i][j].b / (*sample)[i][j] * 255));
    png_write_view("untitled.png", const_view(img));
	*/
}

std::vector<Color> Output::getTile(int x_start, int y_start, int width, int height) const
{
    boost::mutex::scoped_lock lock_r(mutex_read);
	std::vector<Color> ret;
	int x_end = x_start + width;
	int y_end = y_start + height;
	for (unsigned i = y_start; i < y_end; ++i )
		for(unsigned j = x_start; j < x_end; ++j )
			ret.push_back((*image)[i][j]);
	// std::cout<<"in Output::getTile, before return"<<std::endl;
	return ret;
}

void Output::outputTile(int x_start, int y_start, int width, int height) const
{
    boost::mutex::scoped_lock lock_r(mutex_read);
	rgb8_image_t tile(width, height);
	rgb8_view_t my_view = view(tile);
	int x_end = x_start + width;
	int y_end = y_start + height;
	for (unsigned i = y_start; i < y_end; ++i )
		for(unsigned j = x_start; j < x_end; ++j ){
			/*
			my_view(j - x_start,i - y_start) = rgb8_pixel_t(
			             (unsigned char)((*image)[i][j].r / (*sample)[i][j] * 255),
			             (unsigned char)((*image)[i][j].g / (*sample)[i][j] * 255),
            		     (unsigned char)((*image)[i][j].b / (*sample)[i][j] * 255));
						 */
			my_view(j - x_start,i - y_start) = rgb8_pixel_t(
			             (unsigned char)((*image)[i][j].r * 255),
			             (unsigned char)((*image)[i][j].g * 255),
            		     (unsigned char)((*image)[i][j].b * 255));
		}
	std::ostringstream strs;
	strs<<"/tmp/yang-blender/"<<x_start<<"-"<<y_start<<".png";
	std::string str = strs.str();
	png_write_view(str, const_view(tile));
}

void Output::addColor(Color c, int x, int y)
{
    boost::mutex::scoped_lock lock_r(mutex_read);
    boost::mutex::scoped_lock lock_w(mutex_write);
	// c.toRealColor();
	(*image_pool)[y][x] = (*image_pool)[y][x] + c;
	(*sample)[y][x] += 1;
	Color c1 = (*image_pool)[y][x];
	c1.divide((*sample)[y][x]);
	(*image)[y][x] = c1.toRealColor();
	// v(y, x) = rgb8_pixel_t(c.r, c.g, c.b);
}
