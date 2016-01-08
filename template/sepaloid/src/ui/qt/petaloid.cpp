/**
 * @file petaloid.cpp
 * @author
 * @date
 *
 * @brief
 */

#include "petaloid.hpp"
#include <QApplication>
#include <iostream>
#include "MainWindow.hpp"

PFS_PETALOID_API pfs::petaloid * __petaloid_ctor__ (const char *, int argc, char * argv[])
{
	ui::petaloid * p = new ui::petaloid(argc, argv);
	return p;
}

PFS_PETALOID_API void  __petaloid_dtor__ (pfs::petaloid * p)
{
	delete p;
}

namespace ui {

petaloid * petaloid::_self = nullptr;

petaloid::petaloid (int argc, char * argv[])
	: pfs::petaloid("ui::qt")
	, _hppmm(0), _vppmm(0)
	, _argc(argc), _argv(argv)
	, _guiReady(false)
	, _mainWindow(nullptr)
{
	PFS_ASSERT(_self == nullptr);
	this->run = & main_loop; // run petaloid in separate thread
	_self = this;
}

int petaloid::main_loop (pfs::petaloid * ptr)
{
	petaloid * p = dynamic_cast<petaloid *>(ptr);

//	pfs::unitype rcc;
//	__PETALOID->emitGetPref(_l1("rcc-path"), rcc);

//	QResource::registerResource(qcast(rcc.toString()));
	QApplication qapp(p->_argc, p->_argv);
	qapp.setQuitOnLastWindowClosed(true);

	p->_mainWindow = new MainWindow;
	p->_mainWindow->show();

	p->emitGuiReady(true);
	int r = qapp.exec();
	p->emitGuiReady(false);

	delete p->_mainWindow;
	p->_mainWindow = nullptr;

//	QResource::unregisterResource(qcast(rcc.toString()));
	p->emitQuit();
	return r;
}

bool petaloid::onStart ()
{
	pfs::platform::display_data d;
	pfs::platform::displayParameters(& d);

	_hppmm = double(d.pixelsWidth)/double(d.mmWidth);
	_vppmm = double(d.pixelsHeight)/double(d.mmHeight);

	PFS_DEBUG((std::cout << "Screen parameters:" << std::endl
			<< "\tgeometry in pixels                : " << d.pixelsWidth << 'x' << d.pixelsHeight << std::endl
			<< "\tgeometry in millimeters           : " << d.mmWidth << 'x' << d.mmHeight << std::endl
			<< "\tpixels per millimeter horizontally: " << _hppmm << std::endl
			<< "\tpixels per millimeter vertically  : " << _vppmm << std::endl));

	return true;
}

bool petaloid::on_finish ()
{
	return true;
}


void petaloid::onGuiReady (bool ready)
{
	_guiReady = ready;

	if (!_guiReady)
		return;
}

}

