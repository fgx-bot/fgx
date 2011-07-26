
#include <QProcess>
#include <QVBoxLayout>


#include "airports/metarwidget.h"

MetarWidget::MetarWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setWindowIcon(QIcon(":/icon/metar"));

	setMinimumWidth(600);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	txtMetar = new QPlainTextEdit();
	txtMetar->setStyleSheet("font-size: 9pt;");
	mainLayout->addWidget(txtMetar);

}

//=================================================
//= load Metar
//===================================================
void MetarWidget::load_metar(QString apt)
{
	QProcess process;
	QStringList args;
	QString startMetarPath;
	//startMetarPath = "metar";
	args << "-r" << "-v" << apt;

	txtMetar->setPlainText( QString("Loading..\n\nmetar %1").arg(apt) );

#ifdef Q_OS_MAC
    startMetarPath = mainObject->X->fgfs_path();
	startMetarPath.chop(4);
	startMetarPath.append("metar");
    process.start(startMetarPath, args, QIODevice::ReadOnly);
#elif defined(Q_OS_UNIX)
	if ( ! mainObject->X->fgroot_use_default() ) {
		startMetarPath = mainObject->X->fgfs_path();
        startMetarPath.chop(4);
        startMetarPath.append("metar");
    }
	QStringList extra_env = mainObject->X->get_fgfs_env();
    if (extra_env.size()) {
        //= append new env vars
        QStringList env = QProcess::systemEnvironment();
        env << extra_env;
        process.setEnvironment(env);
    }
#endif
	process.start(startMetarPath, args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(3000);
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		Q_UNUSED(error_result);
		//qDebug() <<  error_result;
		txtMetar->setPlainText(ok_result);
	}
}

void MetarWidget::clear_metar()
{
	txtMetar->setPlainText( "" );
}
