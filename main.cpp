﻿#include <QThreadPool>
#include <QApplication>
#include <QFontDatabase>
#include <QDir>
#include <QDebug>
#include <QTranslator>
#include "mainwindow.h"
#include "service/servicefacetory.h"
#include "UI/koalawidgetmanager.h"
#include "UI/logindialog.h"

#pragma execution_character_set("utf-8")

void installTranslators()
{
    //get translator file
    QDir languageDir("language");

    QString strLocale = "en_US";
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
#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
//    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
#endif
    QApplication a(argc, argv);
    installTranslators();

    a.setAttribute(Qt::AA_ShareOpenGLContexts);
    a.setApplicationName(QObject::tr("Intelligent Security Surveillance System"));

    WidgetManagerI *widgetM = new KoalaWidgetManager;
    widgetM->setdefaultStyle(WidgetManagerI::Danyahei);
    ServiceFactoryI *facetory = new ServiceFactory;
    NotifyServiceI *notifySerI = facetory->makeNotifyServiceI(ServiceFactoryI::WebSocket);
    a.setProperty("NotifyServiceI",reinterpret_cast<unsigned long long>(notifySerI));
    a.setProperty("ServiceFactoryI",reinterpret_cast<unsigned long long>(facetory));

//    LoginDialog loginD;
//    loginD.setUserStyle(WidgetManagerI::Danyahei);
//    loginD.resize(400,310);
////    loginD.autoLoginOnce(5000);
//    QDialog::DialogCode returnCode = QDialog::DialogCode(loginD.exec());
//    if(returnCode == QDialog::Rejected){
//        delete wm;
//        return -1;
//    }

    MainWindow w(widgetM);
    w.setWindowFlag(Qt::FramelessWindowHint);
    w.showMaximized();

    QDir dir("font");
    if (dir.exists()){
        QFileInfoList files = dir.entryInfoList(QDir::Files);
        foreach (auto fontInfo, files){ //DINCond-Bold、 PingFang SC Regular
            int fontId = QFontDatabase::addApplicationFont(fontInfo.absoluteFilePath());
            qDebug() << QFontDatabase::applicationFontFamilies(fontId);
        }
    }

    QObject::connect(&a,&QApplication::aboutToQuit,&a,[notifySerI]{
        notifySerI->requestInterruption();
        notifySerI->quit();
        notifySerI->wait();
    });
    notifySerI->start();

    return a.exec();
}
