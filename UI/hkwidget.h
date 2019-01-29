#ifndef HKWIDGET_H
#define HKWIDGET_H

#include "HCNetSDK.h"
#include <QWidget>
#include <QDateTime>
#include <QMap>

struct PlayBackParam
{
    QString dvrIp;
    int dvrPort;
    QString dvrUser;
    QString dvrPassword;
    int playBackChannelId;
    QDateTime startTime;
    QDateTime endTime;
};

struct DeviceInfo
{
    QString ip;
    int port;
    QString postion;
    QString userName;
    QString passWord;
    int realPlayChannelId;
    int playBackChannelId;
};

class HKWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HKWidget(QWidget *parent = nullptr);
    bool startPlayBackByFile(PlayBackParam &parms);
    bool startPlayBackByTime(PlayBackParam &parms);
    void startRealPlay(DeviceInfo &devInfo);
    bool play();
    bool pause();
    bool fastForward();
    bool reWind();
    void stop();
    bool setPlayTime(QDateTime &);
    int  playPos();
    unsigned int getPlayTime();

signals:

public slots:

private:
    int m_curPlayhandle;
    QMap<int,std::tuple<QString,int,QString,QString>> m_recorderMap;
    static void __stdcall exceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
};

#endif // HKWIDGET_H
