QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/communicate/communicate.cpp \
    src/communicate/serversetting.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/signal/def/channel.cpp \
    src/signal/def/sigconst.cpp \
    src/signal/def/sigrealeeg.cpp \
    src/signal/def/sigsin.cpp \
    src/signal/dlg/channelsetting.cpp \
    src/signal/dlg/channelwidget.cpp \
    src/signal/dlg/sigsetting.cpp \
    src/signal/function/BDFHeader.cpp \
    src/signal/function/ReadNeuracleData.cpp \
    src/signal/totalchannelwork.cpp \
    src/timergen/timergen.cpp

HEADERS += \
    src/communicate/communicate.h \
    src/communicate/serversetting.h \
    src/mainwindow.h \
    src/signal/def/channel.h \
    src/signal/def/sigconst.h \
    src/signal/def/sigdef.h \
    src/signal/def/sigrealeeg.h \
    src/signal/def/sigsin.h \
    src/signal/dlg/channelsetting.h \
    src/signal/dlg/channelwidget.h \
    src/signal/dlg/sigsetting.h \
    src/signal/function/BDFHeader.h \
    src/signal/function/ReadNeuracleData.h \
    src/signal/totalchannelwork.h \
    src/timergen/timergen.h

FORMS += \
    src/communicate/serversetting.ui \
    src/mainwindow.ui \
    src/signal/dlg/channelsetting.ui \
    src/signal/dlg/channelwidget.ui \
    src/signal/dlg/sigsetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/resource.qrc
