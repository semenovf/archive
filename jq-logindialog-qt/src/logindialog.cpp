/*
 * LoginDialog.cpp
 *
 *  Created on: Sep 8, 2011
 *      Author: wladt
 */


#include <jq/qt/logindialog.hpp>
#include <QtGui/QFormLayout>
#include <QtGui/QSizePolicy>
#include <jq/global.h>

namespace jq { //JQ_NS_BEGIN

LoginDialog::LoginDialog(validator_f validator, bool loopMode, QWidget* parent)
	: QDialog(parent)
	, m_validator(validator)
    , m_loopMode(loopMode)
	, m_loginOk(false)
{
	this->setModal(true);
	this->setWindowTitle(QSTRING_FROM_CHARARRAY(_Tr("Login")));

	m_pass = new PasswordInput(this);
    m_pass->setMinimumWidth(200);

    m_buttonBox = new QDialogButtonBox(this);
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(/*QDialogButtonBox::Cancel|*/QDialogButtonBox::Ok);

    m_warnImg = new QLabel(this);
    m_warnImg->setPixmap(QPixmap(":/images/exclam.png"));
    m_warnText = new QPlainTextEdit(this);
    m_warnText->setMaximumHeight(100);
    m_warnText->setFrameStyle(QFrame::NoFrame);
    m_warnText->setReadOnly(true);

    // I could not get results by changing the background color in this way (Qt v4.7)
    //
    //m_warnText->setBackgroundRole(QPalette::Window);
    //
    // and this
    //
    //QPalette pal = m_warnText->palette();
    //pal.setColor(m_warnText->foregroundRole(), Qt::blue);
    //m_warnText->setPalette(pal);
    //
    // but works only this way
    QColor bg = this->palette().window().color();
    m_warnText->setStyleSheet(QString("background-color: %1;").arg(bg.name()));

    m_warnText->hide();
    m_warnImg->hide();

	QFormLayout *formLayout = new QFormLayout;
	formLayout->addRow("You are:", new QLabel("anonymous", this));
	formLayout->addRow("Password:", m_pass);
	formLayout->addRow(m_warnImg, m_warnText);
	formLayout->addRow(m_buttonBox);

	formLayout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(formLayout);

	connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_pass, SIGNAL(gotFocus()), this, SLOT(clearLoginFailedWarning()));

	this->resize(sizeHint());
}

LoginDialog::~LoginDialog()
{}


void LoginDialog::showLoginFailedWarning(const jq::String& msg)
{
	m_warnText->setPlainText(QSTRING_FROM_CHARARRAY(msg.c_str()));
	m_warnImg->show();
	m_warnText->show();
}

void LoginDialog::clearLoginFailedWarning()
{
	m_warnText->clear();
	m_warnImg->hide();
	m_warnText->hide();
}

void LoginDialog::closeEvent(QCloseEvent* e)
{
	if( !m_loginOk )
		e->ignore();
}

void LoginDialog::accept()
{
	jq::String user(_T(""));
	jq::String pass( QSTRING_TO_STDSTRING(m_pass->text()));

	LoginDialogAppender logger(this);
	logger.connect();
	m_loginOk = m_validator(user, pass); //emitValidate(user, pass, m_loginOk);
	logger.disconnect();

	if( m_loginOk || !m_loopMode )
		QDialog::accept();
/*
	else
		showLoginFailedWarning();
*/

}

bool LoginDialog::event(QEvent* event)
{
	if( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if( Qt::Key_Escape == keyEvent->key() ) {
        	if( m_loopMode )
        		return false;
        }
	}
	return QDialog::event(event);
}


bool PasswordInput::event(QEvent* event)
{
	if( event->type() == QEvent::FocusIn ) {
		this->clear();
		emit gotFocus();
	}
	return QLineEdit::event(event);
}

} // JQ_NS_END
