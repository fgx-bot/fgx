# #####################################################################
# Automatically generated by qmake (2.01a) Do. Mrz 3 00:45:52 2011
# #####################################################################
TEMPLATE = app
TARGET = fgx
DEPENDPATH += .
INCLUDEPATH += .
QT += network \
    xml \
    xmlpatterns \
    webkit

# Input
HEADERS += xobjects/xsettings.h \
    settings/settingsdialog.h \
    network/networkwidget.h \
    network/mptelnet.h \
    aircraft/aircraftwidget.h \
    airports/airportswidget.h \
    xwidgets/execontrols.h \
    panes/timeweatherwidget.h \
    panes/advancedoptionswidget.h \
    panes/outputpreviewwidget.h \
    panes/coresettingswidget.h \
    xwidgets/xgroupboxes.h \
    xobjects/mainobject.h \
    launcher/launcherwindow.h \
    mpmap/mpmapwidget.h \
    settings/pathssettingswidget.h \
    utilities/statusbar.h \
    xwidgets/xtreewidgetitem.h \
    utilities/utilities.h \
    utilities/messagebox.h \
    airports/importairportsdialog.h \
    utilities/osdab/unzip.h \
    utilities/osdab/zipentry_p.h \
    utilities/osdab/unzip_p.h \
    utilities/osdab/zip_p.h \
    utilities/osdab/zip.h \
    utilities/osdab/zipentry_p.h \
    utilities/osdab/zipglobal.h \
    setupwizard/fgrootpage.h \
    setupwizard/fgexepage.h \
    setupwizard/setupwizard.h
SOURCES += main.cpp \
    xobjects/xsettings.cpp \
    settings/settingsdialog.cpp \
    network/networkwidget.cpp \
    network/mptelnet.cpp \
    aircraft/aircraftwidget.cpp \
    airports/airportswidget.cpp \
    xwidgets/execontrols.cpp \
    panes/timeweatherwidget.cpp \
    panes/advancedoptionswidget.cpp \
    panes/outputpreviewwidget.cpp \
    panes/coresettingswidget.cpp \
    xwidgets/xgroupboxes.cpp \
    xobjects/mainobject.cpp \
    launcher/launcherwindow.cpp \
    mpmap/mpmapwidget.cpp \
    settings/pathssettingswidget.cpp \
    utilities/statusbar.cpp \
    utilities/utilities.cpp \
    utilities/messagebox.cpp \
    airports/importairportsdialog.cpp \
    utilities/osdab/unzip.cpp \
    utilities/osdab/zip.cpp \
    utilities/osdab/zipglobal.cpp \
    setupwizard/setupwizard.cpp \
    setupwizard/fgrootpage.cpp \
    setupwizard/fgexepage.cpp
RESOURCES += resources/icons.qrc \
    resources/artwork.qrc
CONFIG += console
