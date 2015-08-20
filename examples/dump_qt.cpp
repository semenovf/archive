#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <cwt/logger.hpp>
#include <stdio.h>

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    Logger::init();

    if (argc < 2) {
    	Logger::error(_Tr("Need to specify serial device path (name on Windows)"));
    	return -1;
    }

    QSerialPort port;
    if (!port.open(QIODevice::ReadWrite)) {
    	Logger::error("%s: open serial port failed", argv[1]);
    	return -1;
    }
    port.setPortName(argv[1]);
    port.setBaudRate(QSerialPort::Baud2400);
    port.setDataBits(QSerialPort::Data7);
    port.setStopBits(QSerialPort::OneStop);
    port.setParity(QSerialPort::OddParity);
    port.setFlowControl(QSerialPort::SoftwareControl);
    Logger::debug("%s: open serial port ok", argv[1]);
    port.close();

    return 0;
}
