/**
 * @file LogWidget.hpp
 * @author
 * @date
 */

#ifndef __LOGWIDGET_HPP__
#define __LOGWIDGET_HPP__

#include <pfs/string.hpp>
#include <pfs/datetime.hpp>
#include <QTableWidget>

class LogWidget : public QTableWidget
{
	Q_OBJECT

	int     _rowLimit;

	QColor _infoFgColor , _infoBgColor;
	QColor _debugFgColor, _debugBgColor;
	QColor _warnFgColor , _warnBgColor;
	QColor _errorFgColor, _errorBgColor;

public:
	LogWidget (QWidget * parent = nullptr);
	virtual ~LogWidget () {}

private:
	void log (const pfs::string & pixname, QColor fg, QColor bg, pfs::datetime dt, pfs::string msg);

public slots:
	void info  (pfs::datetime dt, pfs::string msg) { log(_l1(":/ico/log-info.png") , _infoFgColor , _infoBgColor , dt, msg); }
	void debug (pfs::datetime dt, pfs::string msg) { log(_l1(":/ico/log-debug.png"), _debugFgColor, _debugBgColor, dt, msg); }
	void warn  (pfs::datetime dt, pfs::string msg) { log(_l1(":/ico/log-warn.png") , _warnFgColor , _warnBgColor , dt, msg); }
	void error (pfs::datetime dt, pfs::string msg) { log(_l1(":/ico/log-error.png"), _errorFgColor, _errorBgColor, dt, msg); }
};

#endif /* __LOGWIDGET_HPP__ */
