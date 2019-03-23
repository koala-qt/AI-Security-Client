#-------------------------------------------------
#
# Project created by QtCreator 2018-09-10T09:54:49
#
#-------------------------------------------------

QT       += core gui webenginewidgets webchannel charts concurrent websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AI-Security-Client
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

include (service/service.pri)
include (UI/ui.pri)
include (dao/dao.pri)
#RC_FILE  = myico.rc
RC_ICONS = appLogo.ico
QMAKE_CXXFLAGS+=-DNOMINMAX

win32{
contains(QT_ARCH, i386):{
LIBS += $$PWD/3rd/curl/lib/windows/win32/libcurl-d_imp.lib \
        $$PWD/3rd/HK_SDK/lib/windows/win32/GdiPlus.lib \
        $$PWD/3rd/HK_SDK/lib/windows/win32/HCCore.lib \
        $$PWD/3rd/HK_SDK/lib/windows/win32/HCNetSDK.lib \
        $$PWD/3rd/HK_SDK/lib/windows/win32/PlayCtrl.lib \
        ws2_32.lib
}else{
INCLUDEPATH += = $$PWD/3rd/curl/include \
                 $$PWD/3rd/HK_SDK/include \
                 $$PWD/3rd/klvideowidget/include \
                 $$PWD/3rd/ffmpeg/x64/include \
                 D:/boost_1_58_0-x64/include \
                 D:/thrift-0.11.0-x64-release/include \
                 D:/mosquitto-x64/include \
                 D:/protobuf-x64-3.6.1-release/include \
                 $$PWD/3rd/milestone\include

CONFIG(debug,debug|release){
LIBS += -LD:/boost_1_58_0-x64/lib \
        -LD:/OpenSSL-Win64/lib/VC \
        -L$$PWD/3rd/HK_SDK/lib/windows/win64 \
        -LD:/thrift-0.11.0-x64/lib \
        -LD:/mosquitto-x64/lib \
        -LD:/protobuf-x64-3.6.1/lib \
        $$PWD/3rd/curl/lib/windows/win64/libcurl-d_imp.lib \
        $$PWD/3rd/klvideowidget/x64/lib/debug/klvideowidget.lib \
        $$PWD/3rd/milestone/lib/x64/ToolkitFactoryProvider.lib \
        GdiPlus.lib HCCore.lib HCCore.lib HCNetSDK.lib PlayCtrl.lib \
        libthrift.lib libthriftnb.lib libcrypto64MDd.lib libssl64MDd.lib ws2_32.lib \
        mosquitto.lib mosquittopp.lib libprotobufd.lib libprotobuf-lited.lib libprotocd.lib
}else{
LIBS += -LD:/boost_1_58_0-x64/lib \
        -LD:/OpenSSL-Win64/lib/VC \
        -L$$PWD/3rd/HK_SDK/lib/windows/win64 \
        -LD:/thrift-0.11.0-x64-release/lib \
        -LD:/mosquitto-x64/lib \
        -LD:/protobuf-x64-3.6.1-release/lib \
        $$PWD/3rd/curl/lib/windows/win64/libcurl-d_imp.lib \
        $$PWD/3rd/klvideowidget/x64/lib/release/klvideowidget.lib \
        $$PWD/3rd/milestone/lib/x64/ToolkitFactoryProvider.lib \
        GdiPlus.lib HCCore.lib HCCore.lib HCNetSDK.lib PlayCtrl.lib \
        libthrift.lib libthriftnb.lib libcrypto64MD.lib libssl64MD.lib ws2_32.lib \
        mosquitto.lib mosquittopp.lib libprotobuf.lib libprotobuf-lite.lib libprotoc.lib
}
}
}
SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

TRANSLATIONS += \
    language/app.en_US.ts \
    language/app.zh_CN.ts

language.files += $$PWD/language/*.qm
language.path = $$PWD/debug/language/
INSTALLS += language
