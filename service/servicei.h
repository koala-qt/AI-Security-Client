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
class NotifyPersonI;
class NotifyEventI;
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
        Kafka,
        WebSocket,
        WebsocketEvent_MqttPerson
    };
    virtual NotifyServiceI* makeNotifyServiceI(NotifyInterfaceType s = WebsocketEvent_MqttPerson) = 0;
    virtual RestServiceI* makeRestServiceI(RestInterfaceType s = HTTPREST) = 0;
    virtual NotifyPersonI* makeNotifyPersonServiceI(NotifyInterfaceType s = Mqtt) = 0;
    virtual NotifyEventI* makeNotifyEventServiceI(NotifyInterfaceType s = WebSocket) = 0;
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
    struct CaptureSearchArgs
    {
        int page;
        int pageCount;
        QString position;
        QStringList faceAttributList;
        QStringList bodyAttributeList;
        QDateTime start;
        QDateTime end;
    };
    struct CameraInfo
    {
        int isMonitor;
        QString cameraId;
        QString cameraPos;
        QString rtsp;
    };
    struct CameraGoup
    {
        QString description;
        QString groupName;
        QString groupNo;
        int deviceNumber;
    };
    struct CameraGoupReturn
    {
        CameraGoup thisInfo;
        QVector<CameraGoup> childGroup;
    };
    struct SceneInfo
    {
        QString sceneId;
        QVector<QPair<QRect,QImage>> faceRectVec;
        QVector<QPair<QRect,QImage>> bodyRectVec;
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
        QStringList  faceAttrs;
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
        qreal lat = 0.0;
        qreal lng = 0.0;
        QString cameraId;
        QString objId;
        QString sceneId;
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
        QStringList faceAttrs;
        QDateTime startT;
        QDateTime endT;
    };
    struct FaceLinkDataCollReturn
    {
        int toatal;
        int totalPage;
        QVector<DataRectureItem> records;
    };
    struct PortraitReturnData
    {
        QString objId;
        QString id;
        QString name;
        QString personType;
        QStringList faceAttrs;
        QStringList bodyAttrs;
        QImage faceImg;
        QImage bodyImg;
    };
    struct EventSearchArgs
    {
        int pageNo;
        int pageSize;
        QString cameraId;
        QString alarmType;
        QString personType;
        QString groupNo;
        QDateTime start;
        QDateTime end;
    };
    struct EventSearchItem
    {
        QString id;
        QString sourceId;
        QString sourceName;
        QString eventType;
        QString sceneId;
        QString bodyId;
        QPolygonF warnZong;
        QDateTime timeStamp;
        QImage image;
    };
    struct EventSearchReturn
    {
        int total;
        int totalPage;
        QVector<EventSearchItem> data;
    };
    struct SearchAttrsArgs
    {
        QString cameraId;
        QString model;
        QStringList faceAttrs;
        QDateTime startT;
        QDateTime endT;
    };
    struct PersonGroupInfo
    {
        int id = -1;
        QString no;
        QString name;
        QString description;
        QVector<PersonGroupInfo> children;
    };
    struct PersonRegisterArgs
    {
        QString name;
        QString id;
        QString taggingKey;
        QString personTypeNo;
        QString personTypeName;
        QString groupNo;
        QString groupName;
        QString describ;
        QImage image;
    };

    // 3.6 add PersonType's children
    struct ChildNoteItem
    {
        int nId;
        QString strNo;
        QString strName;
        QString strDescription;
        QString strNoteHierarchy;
        QString strNoteRequired;
    };
    /**
     * Added by aihc for Portrait library comparison.
     * @brief The PersonType struct
     */
    struct PersonType
    {
        int nId;
        QString strTypeNo;
        QString strTypeName;
        QString groupNo;
        QList<ChildNoteItem> lstChildren;
    };
    /**
     * Added by aihc for Portrait library comparison.
     * @brief The PortraitLibCompArgs struct
     */
    struct PortraitLibCompArgs
    {
        QString sourceType; //从哪里注册的，证件库，标注库
        //int pageNo;
        //int pageSize;
        QString libType; // Not ID.This is the field no.
        int nPersonId;
        QString strPersonName; // Name
        float similarity;
        int limit;
        bool bRequireBase64; // Whether to return image data.
        QImage image;
    };
    /**
     * Added by aihc for Portrait library comparison.
     * @brief The Portrait library comparison struct
     */
    struct PortraitLibCompItem
    {
        QString strPersonName;
        int nPersonId;
        double dSimilarity;
        QString strBigType;
        QString strSubType;
        QImage faceImg; // picture
    };

    // m:n start
    /**
     * Added by aihc for Multiple face analysis.
     * @brief The PortraitLibCompArgs struct
     */
    struct MNFaceAnalysisArgs
    {
        QString sourceType;
        QString strFolderPath;
        int cameraId;
        QDateTime startTime;
        QDateTime endTime;
        QString libType; // Not ID.This is the field no.
        int nPersonId;
        QString strPersonName; // Name
        float similarity;
        int limit; // can not pass the value.
        bool bRequireBase64; // Whether to return image data.
    };
    struct MNCaptureItem
    {
        QImage captureImg;
        int cameraId;
        double dSimilarity;
        QString strPersonName;
        int nPersonId;
        QString strBigType;
        QString strSubType;
    };
    /**
     * Added by aihc for Multiple face analysis.
     * @brief The MNFaceAnalysisItem struct
     */
    struct MNFaceAnalysisItem
    {
        QImage uploadImg;
        bool bSuccess;
        QString strErrMsg;
        QList<MNCaptureItem> captureItems;
    };
    // m:n end

    // global view top statistics
    struct GLViewTopStatistics
    {
        QString strLocationAccess;
        QString strCameraAccess;
        QString strTotalIDNumbers;
        QString strDataStorage;
    };

    RestServiceI(QObject *parent = nullptr):QObject(parent){
        qRegisterMetaType<QVector<RestServiceI::CameraInfo>>("QVector<RestServiceI::CameraInfo>");
        qRegisterMetaType<PagedSnapFaceHis>("PagedSnapFaceHis");
        qRegisterMetaType<QVector<StatisTask>>("QVector<StatisTask>");
        qRegisterMetaType<QVector<QPolygonF>>("QVector<QPolygonF>");
        qRegisterMetaType<RestServiceI::EventSearchReturn>("RestServiceI::EventSearchReturn");
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
        qRegisterMetaType<QVector<QImage>>("QVector<QImage>");
        qRegisterMetaType<RestServiceI::PortraitReturnData>("RestServiceI::PortraitReturnData");
        qRegisterMetaType<QVector<RestServiceI::PersonGroupInfo>>("QVector<RestServiceI::PersonGroupInfo>");
        // Added by aihc begin
        qRegisterMetaType<QVector<RestServiceI::PortraitLibCompItem>>("QVector<RestServiceI::PortraitLibCompItem>");
        qRegisterMetaType<QVector<RestServiceI::PersonType>>("QVector<RestServiceI::PersonType>");
        qRegisterMetaType<QVector<RestServiceI::MNFaceAnalysisItem>>("QVector<RestServiceI::MNFaceAnalysisItem>");
        qRegisterMetaType<GLViewTopStatistics>("GLViewTopStatistics");
        // Added by aihc end
    }
    virtual void getSceneInfo(const QString old,const QString alarmBoyId = QString()) = 0;
    virtual void faceTracking(FaceTrackingArgs) = 0;
    virtual void getFaceLinkDataColl(FaceLinkDataCollArgs &args) = 0;
    virtual void getPersonDetails(QString &) = 0;
    virtual void combinationSearch(CombinationSearchArgs &) = 0;
    virtual void multipleSearch(MultipleSearchArgs &) = 0;
    virtual void getAlarmScenePic(const QString oid) = 0;
    virtual void getImageByUrl(QString &) = 0;
    virtual void getImagesByUrlList(QStringList &) = 0;
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
    virtual void searchAlarmHistory(EventSearchArgs &) = 0;
    virtual void semanticSearch(SemanticSearchArgs &) = 0;
    virtual void searchByImage(SearchUseImageArgs &) = 0;
    virtual void uploadVideo(QString videoPath) = 0;
    virtual void getAvailabelAttrs(SearchAttrsArgs &) = 0;
    virtual void personRegist(PersonRegisterArgs &) = 0;
    virtual void getPersonGoupInfos(QString &groupNo) = 0;
    virtual void cancelRequest() = 0;
    /**
     * Added by aihc for Portrait library comparison.
     * @brief portraitLibCompSearch
     */
    virtual void portraitLibCompSearch(PortraitLibCompArgs &) = 0;
    /**
     * Added by aihc for Portrait library comparison.
     * @brief queryPersonType
     */
    virtual void queryPersonTypes() = 0;
    /**
     * Added by aihc for Multiple face analysis.
     * @brief mnFaceAnalysisSearch
     */
    virtual void mnFaceAnalysisSearch(MNFaceAnalysisArgs &) = 0;

    virtual void queryGLViewTopStatistics(QString strDay) = 0;

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
    void sigDownloadImages(QVector<QImage>);
    void sigStatisInfo(QVector<StatisTask>);
    void sigFaceInfo(QStringList,QImage);
    void sigCameraGroup(QVector<RestServiceI::CameraGoup>);
    void sigCameraInfo(QVector<RestServiceI::CameraInfo>);
    void sigCaptureSearch(RestServiceI::CaptureSearchReturnData);
    void sigAlarmHistory(RestServiceI::EventSearchReturn);
    void sigFaceLinkTree(QJsonObject);
    void sigPeronsDetails(RestServiceI::PortraitReturnData);
    void sigPersonNumbers(int,int);
    void sigSemanticSearch(RestServiceI::SemanticReturnData);
    void sigSearchByImage(QVector<RestServiceI::DataRectureItem>);
    void sigTrackingNew(QVector<TrackingReturnData>);
    void sigCombinationSearch(CombinationSearchReturenData);
    void sigMultipleSearch(QVector<RestServiceI::MultipleSearchItem>);
    void sigFaceLinkDataColl(RestServiceI::FaceLinkDataCollReturn);
    void sigVideoUploadProgress(double,double);
    void sigAvailableAttrs(QStringList);
    void sigPersonGroupInfos(QVector<RestServiceI::PersonGroupInfo>);
    /**
     * Added by aihc for Portrait library comparison.
     * @brief sigPortraitLibCompResult
     */
    void sigPortraitLibCompResult(QVector<RestServiceI::PortraitLibCompItem>);
    /**
     * Added by aihc for Portrait library comparison.
     * @brief sigPersonTypesResult
     */
    void sigPersonTypesResult(QVector<RestServiceI::PersonType>);
    void sigMNFaceAnalysisResult(QVector<RestServiceI::MNFaceAnalysisItem>);

    void sigGLViewTopStatisResult(GLViewTopStatistics);
};

class NotifyPersonI : public QThread
{
    Q_OBJECT
public:
    struct FaceSnapEventData
    {
        QString cameraId;
        QString cameraPos;
        QString sceneId;
        QDateTime snapTime;
        QImage faceImg;
    };
    NotifyPersonI(QObject *parent = nullptr) : QThread(parent){
        qRegisterMetaType<NotifyPersonI::FaceSnapEventData>("NotifyPersonI::FaceSnapEventData");
    }

signals:
    void sigNetWorkError(QString);
    void sigFaceSnap(NotifyPersonI::FaceSnapEventData);
    void sigFaceLinkDataFinished(QString);
    void sigVideoFacePicture(QString,QImage);
};

class NotifyEventI : public QThread
{
    Q_OBJECT
public:
    struct IntruderEventData
    {
        qreal lat;
        qreal lng;
        QPolygonF warnZone;
        QString bodyId;
        QString sceneId;
        QString deviceName;
        QString eventType;
        QString sourceId;
        QDateTime timeStamp;
        QImage sceneImg;
    };
    struct ABDoorEventData
    {
        qreal lat;
        qreal lng;
        QPolygonF warnZone;
        QString bodyId;
        QString sceneId;
        QString deviceName;
        QString eventType;
        QString sourceId;
        QDateTime timeStamp;
        QImage sceneImg;
    };
    struct ClimbEventData
    {
        qreal lat;
        qreal lng;
        QPolygonF warnZone;
        QString bodyId;
        QString deviceName;
        QString eventType;
        QString sceneId;
        QString sourceId;
        QDateTime timeStamp;
        QImage sceneImg;
    };
    struct GatherEventData
    {
        QString deviceName;
        QString eventType;
        qreal lat;
        qreal lng;
        QString sceneId;
        QString sourceId;
        QDateTime timeStamp;
        QImage sceneImg;
    };
    struct PersonEventData
    {
        int personId;
        qreal faceSimilarity;
        qreal lat;
        qreal lng;
        QString deviceId;
        QString deviceName;
        QString personType;
        QString eventType;
        QString faceId;
        QString sceneId;
        QString sourceId;
        QString bodyId;
        QString personTypenName;
        QDateTime timeStamp;
        QImage image;
        QVector<QImage> faceImages;
    };
    NotifyEventI(QObject *parent = nullptr) : QThread(parent){
        qRegisterMetaType<NotifyEventI::IntruderEventData>("NotifyEventI::IntruderEventData");
        qRegisterMetaType<NotifyEventI::ABDoorEventData>("NotifyEventI::ABDoorEventData");
        qRegisterMetaType<NotifyEventI::PersonEventData>("NotifyEventI::PersonEventData");
        qRegisterMetaType<NotifyEventI::GatherEventData>("NotifyEventI::GatherEventData");
        qRegisterMetaType<NotifyEventI::ClimbEventData>("NotifyEventI::ClimbEventData");
    }

signals:
    void sigIntruderEvent(NotifyEventI::IntruderEventData);
    void sigABDoorEventData(NotifyEventI::ABDoorEventData);
    void sigPersonEventData(NotifyEventI::PersonEventData);
    void sigGatherEventData(NotifyEventI::GatherEventData);
    void sigClimbEventData(NotifyEventI::ClimbEventData);
};

class NotifyServiceI : public QObject
{
    Q_OBJECT
public:
    NotifyServiceI(QObject *parent = nullptr):QObject(parent){

    }
    virtual void start() = 0;
    virtual void stop() = 0;

signals:
    void sigNetWorkError(QString);
    void sigFaceSnap(NotifyPersonI::FaceSnapEventData);
    void sigFaceLinkDataFinished(QString);
    void sigIntruderEvent(NotifyEventI::IntruderEventData);
    void sigABDoorEventData(NotifyEventI::ABDoorEventData);
    void sigPersonEventData(NotifyEventI::PersonEventData);
    void sigVideoFacePicture(QString,QImage);
    void sigGatherEventData(NotifyEventI::GatherEventData);
    void sigClimbEventData(NotifyEventI::ClimbEventData);
};
#endif // SERVICEI_H
