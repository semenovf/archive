/**
 * @file display.cpp
 * @author wladt
 * @date Mar 4, 2014
 */

#include <pfs/platform.hpp>
#include <iostream>

int main(int /*argc*/, char ** /*argv*/)
{
	pfs::platform::display_data d;
	pfs::platform::displayParameters(& d);

	std::cout << "geometry in pixels: " << d.pixelsWidth << 'x' << d.pixelsHeight << std::endl
			<< "geometry in millimeters: " << d.mmWidth << 'x' << d.mmHeight << std::endl;

	return 0;
}




