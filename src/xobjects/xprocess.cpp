

#include "xprocess.h"

XProcess::XProcess(MainObject *mob, QString logger_name, QObject *parent) :
    QObject(parent)
{

	mainObject = mob;
	log_name = logger_name;

	process = new QProcess(this);
	process->setProcessChannelMode(QProcess::MergedChannels);
	process->setReadChannel(QProcess::StandardOutput);

	connect(process, SIGNAL(stateChanged(QProcess::ProcessState)),
			this, SLOT(on_process_state_change(QProcess::ProcessState)));

	connect(process, SIGNAL(readyRead()),
			this, SLOT(on_process_output()));

	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(on_process_finished(int, QProcess::ExitStatus)));

	connect(process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(on_process_error(QProcess::ProcessError)));
}

//===============================================================================
//== fgProcess
void XProcess::start(QString command_line, QStringList extra_env)
{
	mainObject->clear_log(log_name);

	if (extra_env.size()) {
		//= append new env vars
		QStringList env = QProcess::systemEnvironment();
		env << extra_env;
		process->setEnvironment(env);
	}
	process->start(command_line);
	process->closeWriteChannel();
}

void XProcess::stop()
{
	process->kill();
	process->waitForFinished();
}



void XProcess::on_process_state_change(QProcess::ProcessState state)
{
	switch (state) {
		case QProcess::Starting:
			emit running(true);
			break;
		case QProcess::Running:
			emit running(true);
			break;
		case QProcess::NotRunning:
			emit running(false);
			break;
	}
}

void XProcess::on_process_output()
{
	QString line = process->readLine();
	while (!line.isEmpty()) {
		mainObject->add_log(log_name, line);
		line = process->readLine();
	}
}

void XProcess::on_process_finished(int code, QProcess::ExitStatus status)
{
	QString message = QString("process exited with code: %1, status: %2\n")
		.arg(code)
		.arg(status == QProcess::NormalExit ? "QProcess::NormalExit" : "QProcess::CrashExit");
	mainObject->add_log(log_name, message);
}

void XProcess::on_process_error(QProcess::ProcessError error)
{
	QString message("$ error: ");

	switch (error) {
		case QProcess::FailedToStart:
			message += "fgfs failed to start; (incorrect path?)";
			break;
		case QProcess::ReadError:
			message += "there was an error reading from the process";
			break;
		case QProcess::UnknownError:
			message += "unknown error";
			break;
		default:
			return;
	}
	mainObject->add_log(log_name, message);
}


int XProcess::get_pid() {
	if(mainObject->settings->runningOs() == XSettings::WINDOWS){
		return 0;
	}
	return process->pid();
}
