#include <fstream>
#include "output.h"

Output::Output(int ww, int hh):
    width(ww), height(hh),
    image(new array_type(boost::extents[height][width]))
{
}

Output::~Output()
{
}

void Output::writePic()
{
	std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < height; ++i)
		for(unsigned j = 0; j < width; ++j){
    		ofs << (unsigned char)((*image)[i][j].r * 255) <<
	    	(unsigned char)((*image)[i][j].g * 255) <<
		    (unsigned char)((*image)[i][j].b * 255);
	    }
	ofs.close();

}

void Output::addColor(Color c, int x, int y)
{
	(*image)[height - y - 1][x] = c;// NOTE here is reversed!
}
