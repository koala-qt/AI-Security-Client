#ifndef SERVICEI_H
#define SERVICEI_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDateTime>
#include <QJsonObject>
#include <QVariantMap>
#include "dao/face_types.h"
#include "dao/face.pb.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
class RestServiceI;
class NotifyServiceI;
class ServiceFactoryI
{
public:
    enum RestInterfaceType{
        HTTPREST,
        THRIFTREST
    };
    enum NotifyInterfaceType{
        Mqtt,
        Kafka
    };
    virtual RestServiceI* makeRestServiceI(RestInterfaceType s = HTTPREST) = 0;
    virtual NotifyServiceI* makeNotifyServiceI(NotifyInterfaceType s = Mqtt) = 0;
};
class RestServiceI : public QObject
{
    Q_OBJECT
public:
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
    struct CombinationSearchArgs
    {
        float similarity;
        int queryCount;
        float tradeoff;
        QString cameraId;
        QDateTime startTime;
        QDateTime endTime;
        QImage img;
    };
    struct CameraGoup
    {
        QString description;
        QString groupName;
        QString groupNo;
        int deviceNumber;
    };
    struct CaptureSearchArgs
    {
        int page;
        int pageCount;
        QString position;
        QDateTime start;
        QDateTime end;
    };
    struct CameraInfo
    {
        QString cameraId;
        QString cameraPos;
        QString rtsp;
    };
    struct SceneInfo
    {
        QString sceneId;
        QVector<QPair<QRect,QImage>> faceRectVec;
        QImage image;
    };
    struct SearchABDoorTimeArg
    {
        QString camera1;
        QString camera2;
        int count;
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
    struct TrackingReturnData
    {
        QString cameraId;
        QString objId;
        QVariantList faceIds;
        QVariantList faceAttr;
        QVariantList bodyIds;
        QVariantList bodyAttribute;
        QDateTime timeIn;
        QDateTime timeOut;
        QImage faceImg;
        QImage bodyImg;
    };
    struct FaceTrackingArgs
    {
        QString oid;
        QImage faceImg;
        float thresh;
        QDateTime startT;
        QDateTime endT;
    };
    struct MultipleSearchArgs
    {
        QVector<QImage> images;
        QString cameraId;
        float similarity;
        int count;
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
    struct SemanticSearchArgs
    {
        int mode;
        int pageNo;
        int pageSize;
        QStringList faceAttributList;
        QStringList bodyAttributeList;
        QString cameraId;
        QDateTime startT;
        QDateTime endT;
    };
    struct SearchUseImageArgs
    {
        int mode;
        int recordsCount;
        float smilarty;
        QImage image;
        QString cameraId;
        QString faceId;
        QDateTime startT;
        QDateTime endT;
    };
    struct DataRectureItem
    {
        QString id;
        QString cameraId;
        QString sceneId;
        QString personId;
        double similarity;
        QDateTime time;
        QImage img;
    };
    struct MultipleSearchItem
    {
        QString cameraId;
        QString sceneId;
        QDateTime time;
        QImage img;
    };
    struct CombinationScoreReturnItem
    {
        QString id;
        QString cameraId;
        QString sceneId;
        QString personId;
        double similarity;
        QDateTime time;
        QImage faceImg;
        QImage bodyImg;
    };
    struct SemanticReturnData
    {
        int toatal;
        int totalPage;
        QVector<DataRectureItem> records;
    };
    struct CombinationSearchReturenData
    {
        QVector<DataRectureItem> faceList;
        QVector<CombinationScoreReturnItem> bodyList;
    };
    struct CaptureSearchReturnData
    {
        int totalPage;
        int totalCount;
        QVector<DataRectureItem> data;
    };
    struct FaceLinkDataCollArgs
    {
        int pageNo;
        int pageSize;
        QString cameraId;
        QDateTime startT;
        QDateTime endT;
    };
    struct FaceLinkDataCollReturn
    {
        int toatal;
        int totalPage;
        QVector<DataRectureItem> records;
    };
    RestServiceI(QObject *parent = nullptr):QObject(parent){
        qRegisterMetaType<QVector<RestServiceI::CameraInfo>>("QVector<RestServiceI::CameraInfo>");
        qRegisterMetaType<PagedSnapFaceHis>("PagedSnapFaceHis");
        qRegisterMetaType<QVector<StatisTask>>("QVector<StatisTask>");
        qRegisterMetaType<QVector<QPolygonF>>("QVector<QPolygonF>");
        qRegisterMetaType<PagedAlarmHis>("PagedAlarmHis");
        qRegisterMetaType<QVector<QPair<int,QPolygonF>>>("QVector<QPair<int,QPolygonF>>");
        qRegisterMetaType<QVector<SearchFace>>("QVector<SearchFace>");
        qRegisterMetaType<QVector<RestServiceI::CameraGoup>>("QVector<RestServiceI::CameraGoup>");
        qRegisterMetaType<RestServiceI::FaceLinkPointData>("RestServiceI::FaceLinkPointData");
        qRegisterMetaType<RestServiceI::DataRectureItem>("RestServiceI::DataRectureItem");
        qRegisterMetaType<RestServiceI::SemanticReturnData>("RestServiceI::SemanticReturnData");
        qRegisterMetaType<QVector<RestServiceI::DataRectureItem>>("QVector<RestServiceI::DataRectureItem>");
        qRegisterMetaType<QVector<TrackingReturnData>>("QVector<TrackingReturnData>");
        qRegisterMetaType<CombinationSearchReturenData>("CombinationSearchReturenData");
        qRegisterMetaType<RestServiceI::CaptureSearchReturnData>("RestServiceI::CaptureSearchReturnData");
        qRegisterMetaType<QVector<RestServiceI::MultipleSearchItem>>("QVector<RestServiceI::MultipleSearchItem>");
        qRegisterMetaType<RestServiceI::SceneInfo>("RestServiceI::SceneInfo");
        qRegisterMetaType<RestServiceI::FaceLinkDataCollReturn>("RestServiceI::FaceLinkDataCollReturn");
        qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
        qRegisterMetaType<QVector<QVector<double> >>("QVector<QVector<double> >");
        qRegisterMetaType<QVector<kf::PieCharData>>("QVector<kf::PieCharData>");
    }
    virtual void getSceneInfo(const QString old) = 0;
    virtual void faceTracking(FaceTrackingArgs) = 0;
    virtual void getFaceLinkDataColl(FaceLinkDataCollArgs &args) = 0;
    virtual void getPersonDetails(QString &) = 0;
    virtual void combinationSearch(CombinationSearchArgs &) = 0;
    virtual void multipleSearch(MultipleSearchArgs &) = 0;
    virtual void getAlarmScenePic(const QString oid) = 0;
    virtual void getImageByUrl(QString &) = 0;
    virtual void generateFaceLink(FaceLinkArgs) = 0;
    virtual void getFaceLinkPoint(QString &) = 0;
    virtual void getFaceLinkTree(QString &) = 0;
    virtual void getCameraInfo() = 0;
    virtual void getCameraGroup(QString gropuNo) = 0;
    virtual void getCameraDevice(QString groupNo) = 0;
    virtual void getStatisInfo() = 0;
    virtual void addStatis(const QString startId,const QString endId) = 0;
    virtual void removeStatis(const QString startId,const QString endId) = 0;
    virtual void captureSearch(CaptureSearchArgs &) = 0;
    virtual void setWaringArea(const QString,const QVector<QPair<int,QPolygonF>> &) = 0;
    virtual void getWaringArea(const QString) = 0;
    virtual void searchAlarmHistory(const int page,const int pageCount, const QString &cameraId,const QString &alarmType,const QDateTime &start,const QDateTime &end) = 0;

    virtual void semanticSearch(SemanticSearchArgs &) = 0;
    virtual void searchByImage(SearchUseImageArgs &) = 0;

signals:
    void sigFaceLinkFinished(QString);
    void sigTracking(QVector<SearchFace>);
    void sigFaceSearch(QVector<SearchFace>);
    void sigResultState(bool);
    void sigError(QString);
    void sigWaringAreas(QVector<QPair<int,QPolygonF>>);
    void sigFaceLinkData(RestServiceI::FaceLinkPointData);
    void sigSceneInfo(RestServiceI::SceneInfo);
    void sigDownloadImage(QImage);
    void sigStatisInfo(QVector<StatisTask>);
    void sigFaceInfo(QStringList,QImage);
    void sigCameraGroup(QVector<RestServiceI::CameraGoup>);
    void sigCameraInfo(QVector<RestServiceI::CameraInfo>);
    void sigCaptureSearch(RestServiceI::CaptureSearchReturnData);
    void sigAlarmHistory(PagedAlarmHis);
    void sigFaceLinkTree(QJsonObject);
    void sigPeronsDetails(QImage,QImage,QStringList,QStringList);
    void sigPersonNumbers(int,int);
    void sigSemanticSearch(RestServiceI::SemanticReturnData);
    void sigSearchByImage(QVector<RestServiceI::DataRectureItem>);
    void sigTrackingNew(QVector<TrackingReturnData>);
    void sigCombinationSearch(CombinationSearchReturenData);
    void sigMultipleSearch(QVector<RestServiceI::MultipleSearchItem>);
    void sigFaceLinkDataColl(RestServiceI::FaceLinkDataCollReturn);
};

class NotifyServiceI : public QThread
{
    Q_OBJECT
public:
    NotifyServiceI(QObject *parent = nullptr) : QThread(parent){

    }

signals:
    void sigNetWorkError(QString);
    void sigInitsized();
    void sigAreaGarphics(QVector<QPointF>);
    void sigAlarmData(int,int,int);
    void sigFaceGrab(int,int,int,int);
    void sigEventStatics(int,int);
    void sigTotalEvent(int,int);
    void sigEventSpider(QVector<QVector<double>>);
    void sigBlackListAlarmScene(QStringList,QImage);
    void sigBlackListAlarmFace(QStringList,QImage);
    void sigIntruderAlarmScene(QStringList,QImage);
    void sigIntruderAlarmFace(QStringList,QImage);
    void sigTimeCost(QMap<QString,QVariant>);
    void sigGrabedPerson(QStringList,QImage);
    void sigPieChart(QVector<kf::PieCharData>);
    void sigABDoorAlarmScene(QStringList,QImage);
    void sigABDoorAlarmFace(QStringList,QImage);
    void sigFaceLinkDataFinished(QString);
};
#endif // SERVICEI_H
