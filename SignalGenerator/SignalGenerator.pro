QT       += core gui
QT       += network serialport charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dlg/ampsetting.cpp \
    Dlg/channelsetting.cpp \
    Dlg/channelwidget.cpp \
    Dlg/serversetting.cpp \
    Dlg/signalgenerator.cpp \
    Dlg/sigsetting.cpp \
    Sig/sigcos.cpp \
    Sig/sigdc.cpp \
    Sig/sigdef.cpp \
    Sig/sigmain.cpp \
    Sig/sigsin.cpp \
    main.cpp

HEADERS += \
    Dlg/ampsetting.h \
    Dlg/channelsetting.h \
    Dlg/channelwidget.h \
    Dlg/serversetting.h \
    Dlg/signalgenerator.h \
    Dlg/sigsetting.h \
    Sig/sigcos.h \
    Sig/sigdc.h \
    Sig/sigdef.h \
    Sig/sigmain.h \
    Sig/sigsin.h

FORMS += \
    Dlg/ampsetting.ui \
    Dlg/channelsetting.ui \
    Dlg/channelwidget.ui \
    Dlg/serversetting.ui \
    Dlg/signalgenerator.ui \
    Dlg/sigsetting.ui \

INCLUDEPATH += Dlg
INCLUDEPATH += Sig

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
