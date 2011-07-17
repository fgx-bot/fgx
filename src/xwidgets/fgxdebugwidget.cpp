
#include <QTabWidget>
#include <QVBoxLayout>

#include "xwidgets/fgxdebugwidget.h"

#include "xobjects/xsettingsmodel.h"

FgxDebugWidget::FgxDebugWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setMinimumWidth(800);
	setMinimumHeight(600);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QTabWidget *tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);





	//== Command Preview and Output
	commandPreviewWidget = new XCommandPrevieWidget(mainObject);
	tabWidget->addTab(commandPreviewWidget, QIcon(":/icon/debug"), "Command View");


	//========================================================
	//= Bottom TEST Buttons
	QHBoxLayout *bottBox = new QHBoxLayout();
	mainLayout->addLayout(bottBox);
	bottBox->addStretch(10);

	QPushButton *buttLoad = new QPushButton();
	buttLoad->setText("Load Ini");
	bottBox->addWidget(buttLoad);
	connect(buttLoad, SIGNAL(clicked()), mainObject->X, SLOT(read_ini()));

	QPushButton *buttSave = new QPushButton();
	buttSave->setText("Write Ini");
	bottBox->addWidget(buttSave);
	connect(buttSave, SIGNAL(clicked()), mainObject->X, SLOT(write_ini()));


	//========================================================
	//= Save last tab
	tabWidget->setCurrentIndex(mainObject->settings->value("fgx_debug_last_tab",0).toInt());
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab(int)));

}

void FgxDebugWidget::on_tab(int idx)
{
	mainObject->settings->setValue("fgx_debug_last_tab", idx);
}
