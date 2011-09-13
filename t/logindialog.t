#include <QtGui>
#include <jq/qt/logindialog.hpp>


static bool loginValidator(const jq::String& /*user*/, const jq::String& pass)
{
	if( pass.compare("password") == 0 ) {
		return true;
	}
	jq_emitError("Bad password! Type 'password' for valid password.");
	return false;
}

JQ_TEST_DEF(logindialog)
    QApplication app(ARGC, ARGV);
    jq::LoginDialog dialog(loginValidator, true, NULL);
    dialog.show();
    return app.exec();
JQ_TEST_END
