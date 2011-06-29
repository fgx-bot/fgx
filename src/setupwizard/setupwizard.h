#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QWizard>
#include <QWizardPage>

#include "setupwizard/fgexepage.h"
#include "setupwizard/fgrootpage.h"
#include "xobjects/mainobject.h"

class SetupWizard : public QWizard
{
Q_OBJECT
public:
	explicit SetupWizard(MainObject *mObject, QWidget *parent = 0);

	MainObject *mainObject;
	QWizardPage *fgExePage;
	QWizardPage *fgRootPage;

signals:

public slots:

};

#endif // SETUPWIZARD_H
