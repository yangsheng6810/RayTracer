#include <fstream>
#include "output.h"
// using namespace boost::gil;

Output::Output(int ww, int hh):
    width(ww), height(hh),
    image(new color_array_type(boost::extents[height][width])),
    sample(new number_array_type(boost::extents[height][width]))
{
    // img = rgb8_image_t(ww, hh);
    // v = view(img);
	// rgb8_pixel_t black(0, 0, 0);
    // fill_pixels(view(img), black);
	Color c("black");
	for (int i = 0; i < width; ++i)
		for(int j = 0; j < height; ++j){
	        (*image)[j][i] = c;
	        (*image)[j][i] = 0;
		}

}

Output::~Output()
{
}

void Output::writePic() const
{
    boost::mutex::scoped_lock lock(mutex_output);
	std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < height; ++i)
		for(unsigned j = 0; j < width; ++j){
    		ofs << (unsigned char)((*image)[i][j].r / (*sample)[i][j] * 255) <<
	    	(unsigned char)((*image)[i][j].g / (*sample)[i][j] * 255) <<
		    (unsigned char)((*image)[i][j].b / (*sample)[i][j] * 255);
	    }
	ofs.close();
	// png_write_view("untitled.png", const_view(img));
}

void Output::addColor(Color c, int x, int y)
{
    boost::mutex::scoped_lock lock(mutex_modify);
	c.toRealColor();
	(*image)[y][x] += c.toRealColor();
	(*sample)[y][x] += 1;
	// v(y, x) = rgb8_pixel_t(c.r, c.g, c.b);
}
