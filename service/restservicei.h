#ifndef RESTSERVICEI_H
#define RESTSERVICEI_H

#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QJsonObject>
#include <QVariantMap>
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
        SearchABDoorTime,
        MultipleSearch,
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
        GetPersonAverageTime,

        SemanticSearch,
        SearchUseImage
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
    struct CameraInfo
    {
        QString cameraId;
        QString cameraPos;
        QString rtsp;
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
    struct SemanticReturnData
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
    }
    virtual void login(const LoginParameter &) = 0;
    virtual void getScenePic(const QString old) = 0;
    virtual void faceTracking(FaceTrackingArgs) = 0;
    virtual void getPersonDetails(QString &) = 0;
    virtual void searchAbDoorTime(SearchABDoorTimeArg &) = 0;
    virtual void multipleSearch(MultipleSearchArgs &) = 0;
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
    void sigSceneImage(QImage);
    void sigStatisInfo(QVector<StatisTask>);
    void sigFaceInfo(QStringList,QImage);
    void sigCameraGroup(QVector<RestServiceI::CameraGoup>);
    void sigCameraInfo(QVector<RestServiceI::CameraInfo>);
    void sigSnapHistory(PagedSnapFaceHis);
    void sigAlarmHistory(PagedAlarmHis);
    void sigFaceLinkTree(QJsonObject);
    void sigPeronsDetails(QImage,QImage,QStringList,QStringList);
    void sigPersonNumbers(int,int);
    void sigSemanticSearch(RestServiceI::SemanticReturnData);
    void sigCameraMap(QVariantMap);
    void sigSearchByImage(QVector<RestServiceI::DataRectureItem>);
    void sigTrackingNew(QVector<TrackingReturnData>);
};

Q_DECLARE_METATYPE(RestServiceI::LoginParameter)
Q_DECLARE_METATYPE(RestServiceI::FaceLinkArgs)
Q_DECLARE_METATYPE(RestServiceI::FaceTrackingArgs)
Q_DECLARE_METATYPE(RestServiceI::AveragePersonTimeArgs)
Q_DECLARE_METATYPE(RestServiceI::PersonsStayArgs)
Q_DECLARE_METATYPE(RestServiceI::SemanticSearchArgs)
Q_DECLARE_METATYPE(RestServiceI::SearchUseImageArgs)
Q_DECLARE_METATYPE(RestServiceI::MultipleSearchArgs)
Q_DECLARE_METATYPE(RestServiceI::SearchABDoorTimeArg)
#endif // RESTSERVICEI_H
