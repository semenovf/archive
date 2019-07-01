#include <QtGui>
#include <jq/qt/logindialog.hpp>


static bool loginValidator(const jq::String& /*user*/, const jq::String& pass)
{
	if( pass.compare(_T("password")) == 0 ) {
		return true;
	}
	jq_emitError(_Tr("Bad password! Type 'password' for valid password."));
	return false;
}

JQ_TEST_DEF(logindialog)
    QApplication app(ARGC, (char**)ARGV);
    jq::LoginDialog dialog(loginValidator, true, NULL);
    dialog.show();
    return app.exec();
JQ_TEST_END
