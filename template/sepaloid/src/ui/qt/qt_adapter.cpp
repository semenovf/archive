/**
 * @file qt_adapter.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"
#include "qcast.hpp"
#include "MainWindow.hpp"
#include <QApplication>
#include <QMessageBox>


void QtAdapter::onCritical (const cwt::critical & critical)
{
	QMessageBox msgBox(qApp->activeWindow());

	msgBox.setWindowTitle(_Tr("Critical"));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setText(qcast(critical.text()));

	const pfs::vector<pfs::string> details = critical.details();

	if (!details.isEmpty()) {
		QString str;
		for (pfs::vector<pfs::string>::const_iterator it = details.cbegin()
				; it != details.cend(); ++it) {
			str += qcast(*it);
			str += "\n";
		}
		msgBox.setDetailedText(str);
	}

	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
}
