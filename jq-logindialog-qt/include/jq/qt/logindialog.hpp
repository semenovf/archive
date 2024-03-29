/*
 * LoginDialog.hpp
 *
 *  Created on: Sep 8, 2011
 *      Author: wladt
 */

#ifndef _JQ_LOGINDIALOG_HPP_
#define _JQ_LOGINDIALOG_HPP_

#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <jq/global.h>
#include <jq/sigslot.hpp>
#include <jq/string.hpp>
#include <jq/logger.hpp>

// http://cpp-namespace.blogspot.com/2011/09/qt-moc-namespace.html
namespace jq { //JQ_NS_BEGIN

class PasswordInput : public QLineEdit
{
	Q_OBJECT
public:
	DLL_API PasswordInput( QWidget* parent )
		: QLineEdit(parent)
	{
		this->setEchoMode(QLineEdit::Password);
	}

	DLL_API virtual ~PasswordInput() {}
	virtual bool event(QEvent* e);

signals:
	void gotFocus();
};

class LoginDialog : public QDialog/*, public sigslot::has_slots<>*/
{
	Q_OBJECT
public:
	typedef bool (*validator_f)(const jq::String& user, const jq::String& pass);

public:
	DLL_API LoginDialog( validator_f validator, bool loopMode = false, QWidget* parent = NULL );
	DLL_API virtual ~LoginDialog();
	DLL_API bool result() const { return m_loginOk; }
	DLL_API void showLoginFailedWarning(const jq::String& msg);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual bool event(QEvent* e);

public /*signals*/:
	sigslot::signal3<jq::String&, jq::String&, bool&> emitValidate; // user, password, ok

private slots:
	void accept();
	DLL_API void clearLoginFailedWarning();

private:
	validator_f       m_validator;
	bool              m_loopMode;
	bool              m_loginOk;
    QDialogButtonBox *m_buttonBox;
    PasswordInput    *m_pass;
    QLabel           *m_warnImg;
    QPlainTextEdit   *m_warnText;
};

class DLL_API LoginDialogAppender : public jq::LogAppender
{
public:
	LoginDialogAppender(LoginDialog *loginDialog)
		: m_loginDialog(loginDialog) {}

public /*slots*/:
	void warn(const jq::String& m)  { if(m_loginDialog) m_loginDialog->showLoginFailedWarning(m); }
	void error(const jq::String& m) { if(m_loginDialog) m_loginDialog->showLoginFailedWarning(m); }
private:
	LoginDialog *m_loginDialog;
};

} //JQ_NS_END

#endif /* _JQ_LOGINDIALOG_HPP_ */
