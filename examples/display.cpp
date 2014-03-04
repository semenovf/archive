/**
 * @file display.cpp
 * @author wladt
 * @date Mar 4, 2014
 */

#include <cwt/platform.hpp>
#include <iostream>

int main(int /*argc*/, char ** /*argv*/)
{
	cwt::platform::display_data d;
	cwt::platform::displayParameters(& d);

	std::cout << "geometry in pixels: " << d.pixelsWidth << 'x' << d.pixelsHeight << std::endl
			<< "geometry in millimeters: " << d.mmWidth << 'x' << d.mmHeight << std::endl;

	return 0;
}




