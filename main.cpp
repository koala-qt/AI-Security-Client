﻿#include <QThreadPool>
#include <QApplication>
#include <QFontDatabase>
#include <QDir>
#include <QDebug>
#include <QTranslator>
#include <QDesktopWidget>
#include <QSettings>
#include <QStackedWidget>
#include "mainwindow.h"
#include "service/servicefacetory.h"
#include "UI/logindialog.h"
#include "UI/hompage.h"
#include "UI/realtimemonitoring.h"
#include "UI/golbalviewwidget.h"

#pragma execution_character_set("utf-8")

void installTranslators()
{
    //get translator file
    QDir languageDir("language");

    QString strLocale = "zh_CN";
    QStringList fileFilter;
    fileFilter << QString("*.%1.qm").arg(strLocale);
    foreach (QFileInfo fileInfo, languageDir.entryInfoList(fileFilter))
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(fileInfo.absoluteFilePath().replace(".qm", ""));
        qApp->installTranslator(translator);
    }
}

int main(int argc, char *argv[])
{
//#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
//#endif
//    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");
    QApplication a(argc, argv);
    installTranslators();

    a.setAttribute(Qt::AA_ShareOpenGLContexts);
    a.setApplicationName(QObject::tr("Intelligent Security Surveillance System"));

    ServiceFactoryI *facetory = new ServiceFactory;
    a.setProperty("ServiceFactoryI",reinterpret_cast<unsigned long long>(facetory));
    NotifyServiceI *serviceI = facetory->makeNotifyServiceI();
    a.setProperty("NotifyServiceI",reinterpret_cast<unsigned long long>(serviceI));

//    LoginDialog loginD;
//    loginD.setUserStyle(WidgetManagerI::Danyahei);
//    loginD.resize(400,310);
////    loginD.autoLoginOnce(5000);
//    QDialog::DialogCode returnCode = QDialog::DialogCode(loginD.exec());
//    if(returnCode == QDialog::Rejected){
//        delete wm;
//        return -1;
//    }

    MainWindow w;
    w.setWindowFlag(Qt::FramelessWindowHint);

    QDesktopWidget *deskTopW = a.desktop();
    int screenCount = deskTopW->screenCount();
    int screenNum = deskTopW->screenNumber(&w);
    QSettings config("config.ini",QSettings::IniFormat);
    if(screenCount == 1){
        w.showMaximized();
    }else if(screenCount == 2){
        w.move(deskTopW->availableGeometry(config.value("ScreenOrder/MainWindow").toInt()).topLeft());
        w.showMaximized();

        GolbalViewWidget *golbalWin = new GolbalViewWidget;
        golbalWin->setWindowFlag(Qt::FramelessWindowHint);
        golbalWin->setGeometry(deskTopW->availableGeometry(config.value("ScreenOrder/GolbalWindow").toInt()));
        golbalWin->show();
    }else if(screenCount == 3){
        w.move(deskTopW->availableGeometry(config.value("ScreenOrder/MainWindow").toInt()).topLeft());
        w.showMaximized();

        GolbalViewWidget *golbalWin = new GolbalViewWidget;
        golbalWin->setWindowFlag(Qt::FramelessWindowHint);
        golbalWin->setGeometry(deskTopW->availableGeometry(config.value("ScreenOrder/GolbalWindow").toInt()));
        golbalWin->show();

        QStackedWidget *monitoringStackedW = new QStackedWidget;
        monitoringStackedW->setWindowFlag(Qt::FramelessWindowHint);
        HomPage *homeW = new HomPage;
        RealtimeMonitoring *videoW = new RealtimeMonitoring;
        monitoringStackedW->addWidget(homeW);
        monitoringStackedW->addWidget(videoW);
        QObject::connect(homeW,&HomPage::sigSwitchBtnClicked,monitoringStackedW,[monitoringStackedW]{monitoringStackedW->setCurrentIndex(1);});
        QObject::connect(videoW,&RealtimeMonitoring::sigSwitchBtnClicked,videoW,[monitoringStackedW]{monitoringStackedW->setCurrentIndex(0);});
        monitoringStackedW->setObjectName(QObject::tr("Surveillance"));
        monitoringStackedW->setGeometry(deskTopW->availableGeometry(config.value("ScreenOrder/MapSuvilianceWindow").toInt()));
        monitoringStackedW->show();
    }

    QDir dir("font");
    if (dir.exists()){
        QFileInfoList files = dir.entryInfoList(QDir::Files);
        foreach (auto fontInfo, files){ //DINCond-Bold、 PingFang SC Regular
            int fontId = QFontDatabase::addApplicationFont(fontInfo.absoluteFilePath());
            qDebug() << QFontDatabase::applicationFontFamilies(fontId);
        }
    }

    QObject::connect(&a,&QApplication::aboutToQuit,&a,[serviceI]{
        delete serviceI;
    });
    serviceI->start();

    return a.exec();
}
