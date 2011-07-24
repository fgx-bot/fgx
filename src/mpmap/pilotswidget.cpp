
#include <QtDebug>


#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNamedNodeMap>


#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


#include <QVBoxLayout>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include "utilities/utilities.h"
#include "xwidgets/xtreewidgetitem.h"
#include "pilotswidget.h"


PilotsWidget::PilotsWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setMinimumWidth(400);


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);



	//========================================
	//= Toolbar
	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar);
	toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	//* Refresh
	QAction *actionRefresh = new QAction(this);
	actionRefresh->setText("Refresh");
	actionRefresh->setIcon(QIcon(":/icon/refresh"));
	toolbar->addAction(actionRefresh);
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(fetch_pilots()));

	//* CheckBox AutoRefresh
	checkBoxAutoRefresh = new QCheckBox("Auto Refresh");
	toolbar->addWidget(checkBoxAutoRefresh);
	checkBoxAutoRefresh->setChecked(mainObject->settings->value("mpxmap_autorefresh_enabled").toBool());
	connect(checkBoxAutoRefresh, SIGNAL(stateChanged(int)), this, SLOT(on_check_autorefresh(int)));

	//* ComboBox HZ
	comboBoxHz = new QComboBox();
	toolbar->addWidget(comboBoxHz);
	for(int sex=1; sex < 10; sex++){
		comboBoxHz->addItem(QString("%1 sec").arg(QString::number(sex)), QString::number(sex));
	}
	int cidx = comboBoxHz->findData(mainObject->settings->value("mpxmap_autorefresh_hz").toString());
	comboBoxHz->setCurrentIndex(cidx == -1 ? 0 : cidx);
	connect(comboBoxHz, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_changed(int)));

	//=============================================
	// Cols Selector
	QToolButton *buttShowColumns = new QToolButton(this);
	buttShowColumns->setText("Show");
	buttShowColumns->setIcon(QIcon(":/icon/select_cols"));
	buttShowColumns->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttShowColumns->setPopupMode(QToolButton::InstantPopup);
	toolbar->addWidget(buttShowColumns);

	QMenu *menuCols = new QMenu();
	buttShowColumns->setMenu(menuCols);

	//= Cols Widget
	QWidget *widgetColsSelecta = new QWidget();
	QVBoxLayout *layCols = new QVBoxLayout();
	widgetColsSelecta->setLayout(layCols);

	buttonGroupCols = new QButtonGroup(this);
	buttonGroupCols->setExclusive(false);
	connect(buttonGroupCols, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_show_cols(QAbstractButton*)));

	mainObject->settings->beginGroup("pilots_widget_cols");

	QCheckBox *chkShowModel = new QCheckBox();
	chkShowModel->setText("Aircraft Type");
	layCols->addWidget(chkShowModel);
	buttonGroupCols->addButton(chkShowModel, C_AIRCRAFT);
	chkShowModel->setChecked(mainObject->settings->value(QString::number(C_AIRCRAFT), "1").toBool());

	QCheckBox *chkShowLatLon = new QCheckBox();
	chkShowLatLon->setText("Lat/Lon");
	layCols->addWidget(chkShowLatLon);
	buttonGroupCols->addButton(chkShowLatLon, C_LAT);
	chkShowLatLon->setChecked(mainObject->settings->value(QString::number(C_LAT), "1").toBool());

	QCheckBox *chkShowAlt = new QCheckBox();
	chkShowAlt->setText("Altitude");
	layCols->addWidget(chkShowAlt);
	buttonGroupCols->addButton(chkShowAlt, C_ALTITUDE);
	chkShowAlt->setChecked(mainObject->settings->value(QString::number(C_ALTITUDE), "1").toBool());

	QCheckBox *chkShowHdg = new QCheckBox();
	chkShowHdg->setText("Heading");
	layCols->addWidget(chkShowHdg);
	buttonGroupCols->addButton(chkShowHdg, C_HEADING);
	chkShowHdg->setChecked(mainObject->settings->value(QString::number(C_HEADING), "1").toBool());

	mainObject->settings->endGroup();

	QWidgetAction *colsWidgetAction = new QWidgetAction(this);
	colsWidgetAction->setDefaultWidget(widgetColsSelecta);
	menuCols->addAction(colsWidgetAction);








	//=========================================================
	//== Tree ( Coeden )
	tree = new QTreeWidget();
	mainLayout->addWidget(tree);

	tree->setRootIsDecorated(false);
	tree->setUniformRowHeights(true);
	tree->setAlternatingRowColors(true);

	tree->header()->setStretchLastSection(true);
	tree->header()->setResizeMode(QHeaderView::Stretch);
	tree->headerItem()->setText(C_CALLSIGN, "Callsign");
	tree->headerItem()->setText(C_AIRCRAFT, "Aircraft");
	tree->headerItem()->setText(C_ALTITUDE, "Alt");
	tree->headerItem()->setText(C_HEADING, "Hdg");
	tree->headerItem()->setText(C_PITCH, "Pitch");
	tree->headerItem()->setText(C_LAT, "Lat");
	tree->headerItem()->setText(C_LON, "Lon");
	tree->headerItem()->setText(C_FLAG, "Flag");

	tree->headerItem()->setTextAlignment(C_ALTITUDE, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_HEADING, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_LAT, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_LON, Qt::AlignRight);

	tree->setColumnHidden(C_PITCH, true);
	//tree->setColumnHidden(C_FLAG, true);

	tree->setColumnHidden(C_AIRCRAFT, !chkShowModel->isChecked());
	tree->setColumnHidden(C_HEADING, !chkShowHdg->isChecked());
	tree->setColumnHidden(C_ALTITUDE, !chkShowAlt->isChecked());
	tree->setColumnHidden(C_LAT, !chkShowLatLon->isChecked());
	tree->setColumnHidden(C_LON, !chkShowLatLon->isChecked());

	tree->setColumnWidth(C_CALLSIGN, 80);
	tree->setColumnWidth(C_AIRCRAFT, 80);
	tree->setColumnWidth(C_ALTITUDE, 50);
	tree->setColumnWidth(C_HEADING, 50);

	tree->setSortingEnabled(true);
	tree->sortByColumn(C_CALLSIGN, Qt::AscendingOrder);

	connect(tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
			this, SLOT(on_item_doubled_clicked(QTreeWidgetItem*,int)));

	//= Status Bar
	statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);
	statusBar->showMessage("Click refresh to load");


	//==========================================================
	//= Initialize Objects
	netMan = new QNetworkAccessManager(this);

	//timer = new QTimer(this);
	//timer->setInterval(comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt());
	//connect(timer, SIGNAL(timeout()), this, SLOT(fetch_pilots()));
	//fetch_pilots();
	if(checkBoxAutoRefresh->isChecked()){
		//fetch_pilots();
	}
}

void PilotsWidget::fetch_pilots()
{
	server_string = "";
	QUrl url("http://mpmap01.flightgear.org/fg_server_xml.cgi?mpserver01.flightgear.org:5001");
	QNetworkRequest request;
	request.setUrl(url );

	//TODO we need to check if already in a request.. ?
	//if(reply && reply->isRunning()){
	//	qDebug() << "request already running???";
	//	return;
	//}
	reply = netMan->get(request);
	connect(reply, SIGNAL( error(QNetworkReply::NetworkError)),
			this, SLOT(on_server_error(QNetworkReply::NetworkError))
	);
	connect(reply, SIGNAL( readyRead()),
			this, SLOT(on_server_ready_read())
	);
	connect(reply, SIGNAL( finished()),
			this, SLOT(on_server_read_finished())
	);
	statusBar->showMessage("Request");

}


//==========================================================
//= Server Events
//==========================================================
void PilotsWidget::on_server_error(QNetworkReply::NetworkError error){
	qDebug() << "error" << error;
	outLog("FGx: PilotsWidget::on_server_error()");
}

void PilotsWidget::on_server_ready_read(){
	QString s(reply->readAll());
	server_string.append(s);
}

//=============================================
// Server call finished.. so parse to tree
//=============================================
/* we do not want to clear the tree and reload as user would loose focus..
   Also the last "communicate tiem means a pilot does not clear of the list"
   A net conection might drop and reappear later = crash or terrain terrain etc..
   SO current plan is to make a time stamp when a new pilot is gr
*/

void PilotsWidget::on_server_read_finished(){
	//qDebug() << "done"; // << server_string;

	statusBar->showMessage("Got Reply");
	//== Loop all ndes and set flag to 1 - item remaining will b enuked
	for(int idx=0; idx << tree->invisibleRootItem()->childCount(); idx++){
		tree->invisibleRootItem()->child(idx)->setText(C_FLAG, "1");
	}
	tree->setUpdatesEnabled(false);

	//= Create Dom Document
	QDomDocument dom;
	dom.setContent(server_string);

	//= get the <fg_server> node
	QDomNodeList nodes = dom.elementsByTagName("marker");
	QStringList list;

	QTreeWidgetItem *item;

	if (nodes.count() > 0){
		for(int idxd =0; idxd < nodes.count(); idxd++){
			if( mainObject->debug_mode && idxd == 5){
				//return; //DEBUG
			}

			QDomNode node = nodes.at(idxd);
			QDomNamedNodeMap attribs =  node.attributes();

			// = check if pilot in list or update
			QList<QTreeWidgetItem *> fitems = tree->findItems(attribs.namedItem("callsign").nodeValue(), Qt::MatchExactly, C_CALLSIGN);
			if(fitems.size() == 0){
				item = new QTreeWidgetItem(tree);
				item->setText(C_CALLSIGN, attribs.namedItem("callsign").nodeValue());
				tree->addTopLevelItem(item);
			}else{
				item = fitems.at(0);
			}

			//== Check for atc
			QString model = QString(attribs.namedItem("model").nodeValue());
			bool tower = false;
			//= Need to catch atc models here?
			if(model.toLower() == "atc" || model.toLower() == "atc2"){
				tower = true;
			}


			item->setText(C_AIRCRAFT, attribs.namedItem("model").nodeValue());
			item->setText(C_ALTITUDE, QString::number(attribs.namedItem("alt").nodeValue().toFloat(), 'f', 0));
			item->setTextAlignment(C_ALTITUDE, Qt::AlignRight);

			item->setText(C_HEADING, QString::number(attribs.namedItem("heading").nodeValue().toFloat(), 'f', 0));
			item->setTextAlignment(C_HEADING, Qt::AlignRight);

			item->setText(C_LAT, attribs.namedItem("lat").nodeValue());
			item->setTextAlignment(C_LAT, Qt::AlignRight);
			item->setText(C_LON, attribs.namedItem("lng").nodeValue());
			item->setTextAlignment(C_LON, Qt::AlignRight);

			item->setText(C_PITCH, attribs.namedItem("pitch").nodeValue());
			item->setText(C_FLAG, "0");

			emit radar(item->text(C_CALLSIGN),
					   item->text(C_LAT),
					   item->text(C_LON),
					   item->text(C_HEADING),
					   item->text(C_ALTITUDE),
					   tower
				);
		}
	}

	//= remove the flagged items
	QList<QTreeWidgetItem *> items = tree->findItems("1", Qt::MatchExactly, C_FLAG);
	for(int idxr=0; idxr << items.count(); idxr++){
		tree->invisibleRootItem()->removeChild(items.at(idxr));
	}
	tree->setUpdatesEnabled(true);


	if(checkBoxAutoRefresh->isChecked()){
		QTimer::singleShot( comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt() * 1000, this, SLOT(fetch_pilots()) );
		statusBar->showMessage(QString("Waiting %1").arg(comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt()));
	}else{
		statusBar->showMessage("Idle");
	}
	//= Resize columns the first time (python does not have infunction statics like this ;-( ))
	/*
	static bool first_time_resize = false;
	if (first_time_resize == false){
		for(int cidx =0; cidx < tree->columnCount(); cidx++){
			tree->resizeColumnToContents(cidx);
		}
		first_time_resize = true;
	}
	*/
}


void PilotsWidget::on_check_autorefresh(int checked){
	mainObject->settings->setValue("mpxmap_autorefresh_enabled", checked);
	if(checked){
		fetch_pilots();
		//timer->start();
	}else{
		//timer->stop();
	}
}

void PilotsWidget::on_combo_changed(int idx){
	Q_UNUSED(idx);
	mainObject->settings->setValue("mpxmap_autorefresh_hz", comboBoxHz->itemData(comboBoxHz->currentIndex()).toString());
	//timer->setInterval(comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt());
}

void PilotsWidget::on_item_doubled_clicked(QTreeWidgetItem *item, int colidx){
	Q_UNUSED(colidx);
	//emit aircraft_selected(item->text(C_CALLSIGN));
	XAero aero(item->text(C_CALLSIGN));
	aero.lat = item->text(C_LAT);
	aero.lon = item->text(C_LON);
	emit aircraft_selected(aero);
}



void PilotsWidget::on_show_cols(QAbstractButton *button)
{
	int col_idx = buttonGroupCols->id(button);
	tree->setColumnHidden(col_idx, !button->isChecked() );
	if(col_idx == C_LAT){
		tree->setColumnHidden(C_LON, !button->isChecked() );
	}
	mainObject->settings->beginGroup("pilots_widget_cols");
	mainObject->settings->setValue(QString::number(col_idx), button->isChecked());
	mainObject->settings->endGroup();
	mainObject->settings->sync();
}
