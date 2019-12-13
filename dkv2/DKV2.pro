#-------------------------------------------------
#
# Project created by QtCreator 2019-09-04T21:33:31
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DKV2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        askdatedlg.cpp \
        csvwriter.cpp \
        dbfield.cpp \
        dbstructure.cpp \
        dbtable.cpp \
        dkdbhelper.cpp \
        filehelper.cpp \
        finhelper.cpp \
        frmjahresabschluss.cpp \
        helper.cpp \
        htmlbrief.cpp \
        itemformatter.cpp \
        jahresabschluss.cpp \
        kreditor.cpp \
        main.cpp \
        mainwindow.cpp \
        sqlhelper.cpp \
        vertrag.cpp

HEADERS += \
        askdatedlg.h \
        csvwriter.h \
        dbfield.h \
        dbstructure.h \
        dbtable.h \
        dkdbhelper.h \
        filehelper.h \
        financaltimespan.h \
        finhelper.h \
        frmjahresabschluss.h \
        helper.h \
        htmlbrief.h \
        itemformatter.h \
        jahresabschluss.h \
        kreditor.h \
        mainwindow.h \
        sqlhelper.h \
        vertrag.h

FORMS += \
        askDateDlg.ui \
        frmjahresabschluss.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../DOCS/Zinsberechnungsmethode.txt \
    ../DOCS/Zinstage.xlsx \
    ../DOCS/remember.txt \
    ../letter.html \
    DOKU \
    remember.txt

RESOURCES += \
    resource.qrc