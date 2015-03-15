/**
 * @file display.cpp
 * @author wladt
 * @date Mar 4, 2014
 */

#include "pfs/platform.hpp"
#include <X11/Xlib.h>

static const char * __default_display_name = ":0";

namespace pfs { namespace platform {

DLL_API void displayParameters (display_data * d)
{
	if (d) {
		Display * x11Display = XOpenDisplay(__default_display_name);

		PFS_ASSERT(x11Display);

		int defaultScreen = DefaultScreen(x11Display);

		PFS_DEBUG(printf("Default screen (display=%s): %d\n", __default_display_name, defaultScreen));

		d->pixelsWidth  = DisplayWidth    (x11Display, defaultScreen);
		d->pixelsHeight = DisplayHeight   (x11Display, defaultScreen);
		d->mmWidth      = DisplayWidthMM  (x11Display, defaultScreen);
		d->mmHeight     = DisplayHeightMM (x11Display, defaultScreen);

		XCloseDisplay(x11Display);
	}
}

}} // pfs::platform
