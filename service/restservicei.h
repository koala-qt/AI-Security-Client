#ifndef RESTSERVICEI_H
#define RESTSERVICEI_H

#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QJsonObject>
#include "dao/face_types.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
class RestServiceI : public QObject
{
    Q_OBJECT
public:
    enum{
        Login = 1,
        GetTop,
        GetCameraInfo,
        GetCameraGroup,
        FaceTracking,
        PersonDetailes,
        GenerateFaceLink,
        GetFaceLinkTreeData,
        SearchFaceLinkPoint,
        GetCameraDevice,
        CaptureSearch,
        GetScenePic,
        GetAlarmScene,
        GetStatis,
        AddStatis,
        RemoveStatis,
        SetWaringArea,
        GetWaringArea,
        GetAlarmHistory,
        GetSnapHistory,
        GetPersonStayNumber,
        GetPersonAverageTime
    };
    struct LoginParameter
    {
        QString ip;
        int port;
        QString host;
        QString userName;
        QString password;
        int authenticationMethod;
        int xprotectProductFamily;
    };
    struct CameraGoup
    {
        QString description;
        QString groupName;
        QString groupNo;
        int deviceNumber;
    };
    struct FaceLinkArgs
    {
        int depth;
        int num;
        float thresh;
        QString oid;
        QDateTime startT;
        QDateTime endT;
        QImage faceImg;
    };
    struct FaceLinkPointData
    {
        int id;
        int nodeProId = -1;
        QString name;
        QString imgUrl;
        QString intimacy;
        QVector<FaceLinkPointData> childrenPoint;
    };
    struct FaceTrackingArgs
    {
        QString oid;
        QImage faceImg;
        float thresh;
        QDateTime startT;
        QDateTime endT;
    };
    struct PersonsStayArgs
    {
        QString cameraId;
        QDateTime startT;
        QDateTime endT;
    };
    struct AveragePersonTimeArgs
    {
        QString cameraId;
        QDateTime startT;
        QDateTime endT;
    };
    RestServiceI(QObject *parent = nullptr):QObject(parent){
        qRegisterMetaType<QVector<CameraInfo>>("QVector<CameraInfo>");
        qRegisterMetaType<PagedSnapFaceHis>("PagedSnapFaceHis");
        qRegisterMetaType<QVector<StatisTask>>("QVector<StatisTask>");
        qRegisterMetaType<QVector<QPolygonF>>("QVector<QPolygonF>");
        qRegisterMetaType<PagedAlarmHis>("PagedAlarmHis");
        qRegisterMetaType<QVector<QPair<int,QPolygonF>>>("QVector<QPair<int,QPolygonF>>");
        qRegisterMetaType<QVector<SearchFace>>("QVector<SearchFace>");
        qRegisterMetaType<QVector<RestServiceI::CameraGoup>>("QVector<RestServiceI::CameraGoup>");
        qRegisterMetaType<RestServiceI::FaceLinkPointData>("RestServiceI::FaceLinkPointData");
    }
    virtual void login(const LoginParameter &) = 0;
    virtual void getScenePic(const QString old) = 0;
    virtual void faceTracking(FaceTrackingArgs) = 0;
    virtual void getPersonDetails(QString &) = 0;
    virtual void getAlarmScenePic(const QString oid) = 0;
    virtual void getTop(const int id) = 0;
    virtual void getPersonStayTotalCount(PersonsStayArgs &) = 0;
    virtual void getAverageTime(AveragePersonTimeArgs &) = 0;
    virtual void generateFaceLink(FaceLinkArgs) = 0;
    virtual void getFaceLinkPoint(QString &) = 0;
    virtual void getFaceLinkTree(QString &) = 0;
    virtual void getCameraInfo() = 0;
    virtual void getCameraGroup(QString gropuNo) = 0;
    virtual void getCameraDevice(QString groupNo) = 0;
    virtual void getStatisInfo() = 0;
    virtual void addStatis(const QString startId,const QString endId) = 0;
    virtual void removeStatis(const QString startId,const QString endId) = 0;
    virtual void captureSearch(QString &dataBasename,const int page,const int pageCount, const QString &position, const QDateTime &start, const QDateTime &end) = 0;
    virtual void setWaringArea(const QString,const QVector<QPair<int,QPolygonF>> &) = 0;
    virtual void getWaringArea(const QString) = 0;
    virtual void searchAlarmHistory(const int page,const int pageCount, const QString &cameraId,const QString &alarmType,const QDateTime &start,const QDateTime &end) = 0;
    virtual void searchSnap(const QString &dataBasename,const QImage &img,const QString &oid,const QString &cameraId,const int topK,double similarty,QDateTime &start,QDateTime &end) = 0;
	
signals:
    void sigTracking(QVector<SearchFace>);
    void sigFaceSearch(QVector<SearchFace>);
    void sigResultState(bool);
    void sigError(QString);
    void sigWaringAreas(QVector<QPair<int,QPolygonF>>);
    void sigFaceLinkData(RestServiceI::FaceLinkPointData);
    void sigSceneImage(QImage);
    void sigStatisInfo(QVector<StatisTask>);
    void sigFaceInfo(QStringList,QImage);
    void sigCameraGroup(QVector<RestServiceI::CameraGoup>);
    void sigCameraInfo(QVector<CameraInfo>);
    void sigSnapHistory(PagedSnapFaceHis);
    void sigAlarmHistory(PagedAlarmHis);
    void sigFaceLinkTree(QJsonObject);
    void sigPeronsDetails(QImage,QImage,QStringList);
    void sigPersonNumbers(int);
};

Q_DECLARE_METATYPE(RestServiceI::LoginParameter)
Q_DECLARE_METATYPE(RestServiceI::FaceLinkArgs)
Q_DECLARE_METATYPE(RestServiceI::FaceTrackingArgs)
Q_DECLARE_METATYPE(RestServiceI::AveragePersonTimeArgs)
Q_DECLARE_METATYPE(RestServiceI::PersonsStayArgs)
#endif // RESTSERVICEI_H
