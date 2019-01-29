#include <functional>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "hkwidget.h"

HKWidget::HKWidget(QWidget *parent) : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background,Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

bool HKWidget::startPlayBackByFile(PlayBackParam &parms)
{
    NET_DVR_Init();
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);

    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, parms.dvrIp.toStdString().data()); //设备IP地址
    struLoginInfo.wPort = parms.dvrPort; //设备服务端口
    strcpy(struLoginInfo.sUserName, parms.dvrUser.toStdString().data()); //设备登录用户名
    strcpy(struLoginInfo.sPassword, parms.dvrPassword.toStdString().data()); //设备登录密码

    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    int userId = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    NET_DVR_DEVICEINFO_V30 devV30 = struDeviceInfoV40.struDeviceV30;
    int byStartDChan = devV30.byStartDChan; //起始Ip通道号
    int byMatxChan = byStartDChan + (devV30.byIPChanNum + devV30.byHighDChanNum * 256) - 1;
    int currentChId = byStartDChan + parms.playBackChannelId;
    if(byMatxChan < currentChId){
        currentChId = -1;
    }

    if (userId < 0)
    {
        int lastError = NET_DVR_GetLastError();
        qDebug() << "NET_DVR_Login_V40 failed, error code: " << lastError;
        NET_DVR_Cleanup();
        QMessageBox::information(this,QStringLiteral("回放"), QString("NET_DVR_Login_V40 failed, error code: %1").arg(lastError));
        return false;
    }

    NET_DVR_FILECOND_V40 struFileCond={0};
    struFileCond.dwFileType = 0xFF;
    struFileCond.lChannel = currentChId;
    struFileCond.dwIsLocked = 0xFF;
    struFileCond.dwUseCardNo = 0;
    struFileCond.struStartTime.dwYear   = parms.startTime.date().year();
    struFileCond.struStartTime.dwMonth  = parms.startTime.date().month();
    struFileCond.struStartTime.dwDay    = parms.startTime.date().day();
    struFileCond.struStartTime.dwHour   = parms.startTime.time().hour();
    struFileCond.struStartTime.dwMinute = parms.startTime.time().minute();
    struFileCond.struStartTime.dwSecond = parms.startTime.time().second();
    struFileCond.struStopTime.dwYear    = parms.endTime.date().year();
    struFileCond.struStopTime.dwMonth   = parms.endTime.date().month();
    struFileCond.struStopTime.dwDay     = parms.endTime.date().day();
    struFileCond.struStopTime.dwHour    = parms.endTime.time().hour();
    struFileCond.struStopTime.dwMinute  = parms.endTime.time().minute();
    struFileCond.struStopTime.dwSecond  = parms.endTime.time().second();
    int lFindHandle = NET_DVR_FindFile_V40(userId, &struFileCond);
    if(lFindHandle < 0)
    {
        qDebug() << "find file fail,last error " << NET_DVR_GetLastError();
        return false;
    }

    NET_DVR_FINDDATA_V40 struFileData;
    while(true)
    {
        int result = NET_DVR_FindNextFile_V40(lFindHandle, &struFileData);
        if(result == NET_DVR_ISFINDING)
        {
            continue;
        }
        else if(result == NET_DVR_FILE_SUCCESS)
        {
            qDebug() << struFileData.sFileName;
            NET_DVR_RECORD_TIME_SPAN_INQUIRY sargs{0};
            sargs.dwSize = sizeof(sargs);
            sargs.byType = 0;
            NET_DVR_RECORD_TIME_SPAN  result{0};
            NET_DVR_InquiryRecordTimeSpan(userId,currentChId,&sargs,&result);
            qDebug() << parms.startTime.toString("yyyy-MM-dd HH:mm:ss");
            qDebug() << result.strBeginTime.dwYear << result.strBeginTime.dwMonth << result.strBeginTime.dwDay << result.strBeginTime.dwHour << result.strBeginTime.dwMinute << result.strBeginTime.dwSecond;
            qDebug() << result.strEndTime.dwYear << result.strEndTime.dwMonth << result.strEndTime.dwDay << result.strEndTime.dwHour << result.strEndTime.dwMinute << result.strEndTime.dwSecond;
            m_curPlayhandle = NET_DVR_PlayBackByName(userId,struFileData.sFileName,reinterpret_cast<HWND>(winId()));
            if(m_curPlayhandle < 0)
            {
                int lastError = NET_DVR_GetLastError();
                qDebug()<<"NET_DVR_PlayBackByTime_V40 fail,last error " << lastError;
                NET_DVR_StopPlayBack(m_curPlayhandle);
                NET_DVR_Cleanup();
                QMessageBox::information(this,QStringLiteral("回放"), QString("NET_DVR_PlayBackByTime_V40 fail,error code: %1").arg(lastError));
                return false;
            }

            //开始
            NET_DVR_TIME startTime;
            startTime.dwYear = 2018;
            startTime.dwMonth = 9;
            startTime.dwDay = 24;
            startTime.dwHour = 23;
            startTime.dwMinute = 26;
            startTime.dwSecond = 50;
            if(!NET_DVR_PlayBackControl_V40(m_curPlayhandle, NET_DVR_PLAYSTART, &startTime, sizeof(startTime), NULL,0))
            {
                qDebug()<<"play back control failed " << NET_DVR_GetLastError();
                NET_DVR_StopPlayBack(m_curPlayhandle);
                NET_DVR_Cleanup();
                return false;
            }
            break;
        }
        else if(result == NET_DVR_FILE_NOFIND || result == NET_DVR_NOMOREFILE)
        {
            break;
        }
        else
        {
            qDebug() << "find file fail for illegal get file state";
            break;
        }
    }

    return true;
}

bool HKWidget::startPlayBackByTime(PlayBackParam &parms)
{
    NET_DVR_Init();
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);

    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, parms.dvrIp.toStdString().data()); //设备IP地址
    struLoginInfo.wPort = parms.dvrPort; //设备服务端口
    strcpy(struLoginInfo.sUserName, parms.dvrUser.toStdString().data()); //设备登录用户名
    strcpy(struLoginInfo.sPassword, parms.dvrPassword.toStdString().data()); //设备登录密码

    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    int userId = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    NET_DVR_DEVICEINFO_V30 devV30 = struDeviceInfoV40.struDeviceV30;
    int byStartDChan = devV30.byStartDChan; //起始Ip通道号
    int byMatxChan = byStartDChan + (devV30.byIPChanNum + devV30.byHighDChanNum * 256) - 1;
    int currentChId = byStartDChan + parms.playBackChannelId;
    if(byMatxChan < currentChId){
        currentChId = -1;
    }

    if (userId < 0)
    {
        int lastError = NET_DVR_GetLastError();
        qDebug() << "NET_DVR_Login_V40 failed, error code: " << lastError;
        NET_DVR_Cleanup();
        QMessageBox::information(this,QStringLiteral("回放"), QString("NET_DVR_Login_V40 failed, error code: %1").arg(lastError));
        return false;
    }

    NET_DVR_VOD_PARA struVodPara={0};
    struVodPara.dwSize=sizeof(struVodPara);
    struVodPara.struIDInfo.dwSize=sizeof(NET_DVR_STREAM_INFO);
    struVodPara.struIDInfo.dwChannel= currentChId;
    struVodPara.hWnd= reinterpret_cast<HWND>(winId());

    struVodPara.struBeginTime.dwYear   = parms.startTime.date().year();
    struVodPara.struBeginTime.dwMonth  = parms.startTime.date().month();
    struVodPara.struBeginTime.dwDay    = parms.startTime.date().day();
    struVodPara.struBeginTime.dwHour   = parms.startTime.time().hour();
    struVodPara.struBeginTime.dwMinute = parms.startTime.time().minute();
    struVodPara.struBeginTime.dwSecond = parms.startTime.time().second();
    struVodPara.struEndTime.dwYear    =  parms.endTime.date().year();
    struVodPara.struEndTime.dwMonth   =  parms.endTime.date().month();
    struVodPara.struEndTime.dwDay     =  parms.endTime.date().day();
    struVodPara.struEndTime.dwHour    =  parms.endTime.time().hour();
    struVodPara.struEndTime.dwMinute  =  parms.endTime.time().minute();
    struVodPara.struEndTime.dwSecond  =  parms.endTime.time().second();

    //按时间回放
    m_curPlayhandle = NET_DVR_PlayBackByTime_V40(userId, &struVodPara);
    if(m_curPlayhandle < 0)
    {
        int lastError = NET_DVR_GetLastError();
        qDebug()<<"NET_DVR_PlayBackByTime_V40 fail,last error " << lastError;
        NET_DVR_StopPlayBack(m_curPlayhandle);
        NET_DVR_Cleanup();
        QMessageBox::information(this,QStringLiteral("回放"), QString("NET_DVR_PlayBackByTime_V40 fail,error code: %1").arg(lastError));
        return false;
    }

    //开始
    if(!NET_DVR_PlayBackControl_V40(m_curPlayhandle, NET_DVR_PLAYSTART,NULL, 0, NULL,NULL))
    {
        qDebug()<<"play back control failed " << NET_DVR_GetLastError();
        NET_DVR_StopPlayBack(m_curPlayhandle);
        NET_DVR_Cleanup();
        return false;
    }
    return true;
}

void HKWidget::startRealPlay(DeviceInfo &devInfo)
{
    NET_DVR_Init();
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    //    设置异常消息回调函数
    NET_DVR_SetExceptionCallBack_V30(0, NULL, HKWidget::exceptionCallBack, NULL);
    LONG lUserID;
    //    登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, devInfo.ip.toStdString().data()); //设备IP地址
    struLoginInfo.wPort = devInfo.port; //设备服务端口
    strcpy(struLoginInfo.sUserName, devInfo.userName.toStdString().data()); //设备登录用户名
    strcpy(struLoginInfo.sPassword, devInfo.passWord.toStdString().data()); //设备登录密码

    //    设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (lUserID < 0)
    {
        qDebug() << "Login failed, error code: %d" << NET_DVR_GetLastError();
        NET_DVR_Cleanup();
    }

    //启动预览并设置回调数据流
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.hPlayWnd = reinterpret_cast<HWND>(winId());         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel     = devInfo.realPlayChannelId;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

    LONG lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
    if (lRealPlayHandle < 0)
    {
        qDebug() << "NET_DVR_RealPlay_V40 error" << NET_DVR_GetLastError();
        NET_DVR_Cleanup();
    }
    setUpdatesEnabled(false);
}

bool HKWidget::play()
{
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYNORMAL,nullptr,0,nullptr,0);
    if(!resCode){
        qDebug() << "play control failed";
    }
    return resCode;
}

bool HKWidget::pause()
{
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYPAUSE,nullptr,0,nullptr,0);
    if(!resCode){
        qDebug() << "play control failed";
    }
    return resCode;
}

bool HKWidget::fastForward()
{
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYFAST,nullptr,0,nullptr,0);
    if(!resCode){
        qDebug() << "play control failed";
    }
    return resCode;
}

bool HKWidget::reWind()
{
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYSLOW,nullptr,0,nullptr,0);
    if(!resCode){
        qDebug() << "play control failed";
    }
    return resCode;
}

void HKWidget::stop()
{
    NET_DVR_StopPlayBack(m_curPlayhandle);
    NET_DVR_Cleanup();
}

bool HKWidget::setPlayTime(QDateTime &tm)
{
    NET_DVR_TIME args;
    args.dwYear = tm.date().year();
    args.dwMonth = tm.date().month();
    args.dwDay = tm.date().day();
    args.dwHour = tm.time().hour();
    args.dwMinute = tm.time().minute();
    args.dwSecond = tm.time().second();
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYSETTIME,&args,sizeof(args),nullptr,0);
    if(!resCode){
        qDebug() << "play control failed";
    }
    return resCode;
}

int HKWidget::playPos()
{
    int pos = 0;
    DWORD argSize = sizeof(pos);
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYGETPOS,nullptr,0,&pos,&argSize);
    if(!resCode){
        qDebug() << "play control failed" << NET_DVR_GetLastError();
    }
    return pos;
}

unsigned int HKWidget::getPlayTime()
{
    unsigned int time = 0;
    DWORD argSize = sizeof(time);
    bool resCode = NET_DVR_PlayBackControl_V40(m_curPlayhandle,NET_DVR_PLAYGETTIME,nullptr,0,&time,&argSize);
    if(!resCode){
        qDebug() << "play control failed" << NET_DVR_GetLastError();
    }
    return time;
}

void __stdcall HKWidget::exceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    Q_UNUSED(lUserID);
    Q_UNUSED(lHandle);
    Q_UNUSED(pUser);
    switch(dwType)
    {
    case EXCEPTION_RECONNECT:    //预览时重连
        qDebug() << "----------reconnect--------";
        break;
    default:
        break;
    }
}
