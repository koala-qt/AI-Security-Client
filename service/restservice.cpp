#include <QBuffer>
#include <QThread>
#include <QSettings>
#include <QDebug>
#include "core/workermanagerapi.h"
#include "Toolkits/ToolkitInterface.h"
#include "Tools/ServerCommandServiceClient.h"
#include "Toolkits/ToolkitFactoryProvider.h"
#include "UI/milestoneshareddata.h"
#include "restservice.h"
#include "dao/thriftdao.h"
#include "dao/cloudhttpdao.h"
#define NEWSTRCUTER

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

BLL::RestService::RestService(BLL::WorkerManager *wm, QObject *parent):
    RestServiceI(parent),
    BaseWorker(wm)
{

}

BLL::WorkerType BLL::RestService::workerType() const
{
    return RunOnce;
}

void BLL::RestService::login(const RestServiceI::LoginParameter &args)
{
    pushBackTask(Login,QVariant::fromValue(args));
}

void BLL::RestService::combinationSearch(RestServiceI::CombinationSearchArgs &args)
{
    pushBackTask(CombinationSearch,QVariant::fromValue(args));
}

void BLL::RestService::getScenePic(const QString old)
{
    pushBackTask(GetScenePic,QVariant::fromValue(old));
}

void BLL::RestService::faceTracking(RestServiceI::FaceTrackingArgs args)
{
    pushBackTask(FaceTracking,QVariant::fromValue(args));
}

void BLL::RestService::searchAbDoorTime(RestServiceI::SearchABDoorTimeArg &args)
{
    pushBackTask(SearchABDoorTime,QVariant::fromValue(args));
}

void BLL::RestService::multipleSearch(RestServiceI::MultipleSearchArgs &args)
{
    pushBackTask(MultipleSearch,QVariant::fromValue(args));
}

void BLL::RestService::getPersonDetails(QString &objId)
{
    pushBackTask(PersonDetailes,QVariant::fromValue(objId));
}

void BLL::RestService::getAlarmScenePic(const QString oid)
{
    pushBackTask(GetAlarmScene,QVariant::fromValue(oid));
}

void BLL::RestService::getFaceLinkTree(QString &objectID)
{
    pushBackTask(GetFaceLinkTreeData,QVariant::fromValue(objectID));
}

void BLL::RestService::getPersonStayTotalCount(RestServiceI::PersonsStayArgs &args)
{
    pushBackTask(GetPersonStayNumber,QVariant::fromValue(args));
}

void BLL::RestService::getAverageTime(RestServiceI::AveragePersonTimeArgs &args)
{
    pushBackTask(GetPersonAverageTime,QVariant::fromValue(args));
}

void BLL::RestService::getTop(const int id)
{

}

void BLL::RestService::getCameraInfo()
{
    pushBackTask(GetCameraInfo,QVariant());
}

void BLL::RestService::generateFaceLink(RestServiceI::FaceLinkArgs args)
{
    pushBackTask(GenerateFaceLink,QVariant::fromValue(args));
}

void BLL::RestService::getFaceLinkPoint(QString &faceLinkId)
{
    pushBackTask(SearchFaceLinkPoint,QVariant::fromValue(faceLinkId));
}

void BLL::RestService::getCameraGroup(QString gropuNo)
{
    pushBackTask(GetCameraGroup,QVariant::fromValue(gropuNo));
}

void BLL::RestService::getCameraDevice(QString groupNo)
{
    pushBackTask(GetCameraDevice,QVariant::fromValue(groupNo));
}

void BLL::RestService::addStatis(const QString startId, const QString endId)
{
    pushBackTask(AddStatis,QVariant::fromValue(QPair<QString,QString>{startId,endId}));
}

void BLL::RestService::removeStatis(const QString startId, const QString endId)
{
    pushBackTask(RemoveStatis,QVariant::fromValue(QPair<QString,QString>{startId,endId}));
}

void BLL::RestService::getStatisInfo()
{
    pushBackTask(GetStatis,QVariant());
}

void BLL::RestService::captureSearch(CaptureSearchArgs &args)
{
    pushBackTask(CaptureSearch,QVariant::fromValue(args));
}

void BLL::RestService::getWaringArea(const QString cameraId)
{
    pushBackTask(GetWaringArea,QVariant::fromValue(cameraId));
}

void BLL::RestService::searchAlarmHistory(const int page, const int pageCount, const QString &cameraId, const QString &alarmType, const QDateTime &start, const QDateTime &end)
{
    pushBackTask(GetAlarmHistory,QVariant::fromValue(alarmHistoryArg{page,pageCount,cameraId,alarmType,start,end}));
}

void BLL::RestService::semanticSearch(RestServiceI::SemanticSearchArgs &args)
{
    pushBackTask(SemanticSearch,QVariant::fromValue(args));
}

void BLL::RestService::searchByImage(RestServiceI::SearchUseImageArgs &args)
{
    pushBackTask(SearchUseImage,QVariant::fromValue(args));
}

void BLL::RestService::setWaringArea(const QString cameraId, const QVector<QPair<int, QPolygonF> > &polygons)
{
    pushBackTask(SetWaringArea,QVariant::fromValue(qMakePair(cameraId,polygons)));
}

void BLL::RestService::run()
{
    std::string errorStr;
    ThriftDao thriftDaoObj;
    std::pair<int,QVariant> args = getTask(true);
    int argType = args.first;

    if(argType == GetTop){

    }else if(argType == GetCameraInfo){
        DLL::CloudHttpDao httpDao;
        QVector<CameraInfo> cameras;
        QString qerrorStr = httpDao.getCameraInfo(cameras);
        if(qerrorStr.isEmpty()){
            emit sigCameraInfo(cameras);
        }
    }else if(argType == CombinationSearch){
        DLL::CloudHttpDao httpDao;
        QString qerrorStr = httpDao.combinationSearch(args.second.value<RestServiceI::CombinationSearchArgs>());
        if(qerrorStr.isEmpty()){
//            emit sigCameraGroup(resGroup);
        }else{
            emit sigError(qerrorStr);
        }
    }else if(argType == GetCameraGroup){
        DLL::CloudHttpDao httpDao;
        QVector<RestServiceI::CameraGoup> resGroup;
        QString qerrorStr = httpDao.getGroup(args.second.toString(),resGroup);
        if(qerrorStr.isEmpty()){
            emit sigCameraGroup(resGroup);
        }
    }else if(argType == GetCameraDevice){
        DLL::CloudHttpDao httpDao;
        QVector<CameraInfo> devices;
        QString qerrorStr = httpDao.getDevice(args.second.toString(),devices);
        if(qerrorStr.isEmpty()){
            emit sigCameraInfo(devices);
        }
    }else if(argType == GenerateFaceLink){
        DLL::CloudHttpDao httpDao;
#ifdef NEWSTRCUTER
        QString finishId;
        QString qerrorStr = httpDao.faceLink_(args.second.value<FaceLinkArgs>(),finishId);
        if(qerrorStr.isEmpty()){
            emit sigFaceLinkFinished(finishId);
        }else{
            emit sigError(qerrorStr);
        }
#else
        QString qerrorStr = httpDao.faceLink(faceArgs);
#endif
    }else if(argType == SearchFaceLinkPoint){
        DLL::CloudHttpDao httpDao;
        FaceLinkPointData rootPointdata;
        QString qerrorStr = httpDao.getFaceLinkPoint(args.second.toString(),rootPointdata);
        emit sigFaceLinkData(rootPointdata);
    }else if(argType == GetFaceLinkTreeData){
        DLL::CloudHttpDao httpDao;
        QJsonObject jsObj;
        QString qerrorStr = httpDao.getFaceLinkTree(args.second.toString(),jsObj);
        if(qerrorStr.isEmpty()){
            emit sigFaceLinkTree(jsObj);
        }
    }else if(argType == PersonDetailes){
        DLL::CloudHttpDao httDao;
        QImage face,body;
        QStringList attrsface,attrsbody;
        QString qerrorStr = httDao.getPersonDetailes(args.second.toString(),face,body,attrsface,attrsbody);
        if(qerrorStr.isEmpty()){
            emit sigPeronsDetails(face,body,attrsface,attrsbody);
        }else{
            emit sigError(qerrorStr);
        }
    }else if(argType == FaceTracking){
#ifdef NEWSTRCUTER
        DLL::CloudHttpDao httDao;
        QVector<TrackingReturnData> resVec;
        QString qerrorStr = httDao.tracking(args.second.value<FaceTrackingArgs>(),resVec);
        if(qerrorStr.isEmpty()){
            emit sigTrackingNew(resVec);
        }else{
            emit sigError(qerrorStr);
        }
#else
        FaceTrackingArgs faceArgs = args.second.value<FaceTrackingArgs>();
        QByteArray imgStr;
        QBuffer buf(&imgStr);
        faceArgs.faceImg.save(&buf);
        std::vector<SearchFace> trackingRes = thriftDaoObj.generateTrackingByFace(faceArgs.oid.toStdString(),
                                                                                  imgStr.toStdString(),
                                                                                  faceArgs.thresh,
                                                                                  faceArgs.startT.toMSecsSinceEpoch(),
                                                                                  faceArgs.endT.toMSecsSinceEpoch(),
                                                                                  errorStr);
        if(errorStr.empty()){
            emit sigTracking(QVector<SearchFace>::fromStdVector(trackingRes));
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
#endif
    }else if(argType == CaptureSearch){
        DLL::CloudHttpDao httpDao;
        RestServiceI::CaptureSearchReturnData resdatas;
        QString qerrorStr = httpDao.captureSearch(args.second.value<CaptureSearchArgs>(),resdatas);
        if(qerrorStr.isEmpty()){
            emit sigCaptureSearch(resdatas);
        }else{
            emit sigError(qerrorStr);
        }
    }else if(argType == GetScenePic){
#ifdef NEWSTRCUTER
        DLL::CloudHttpDao httpDao;
        QImage img;
        QString qerrorStr = httpDao.getScenePic(args.second.toString(),img);
        if(qerrorStr.isEmpty()){
            emit sigSceneImage(img);
        }else{
            sigError(qerrorStr);
        }
#else
        std::string imgStr = thriftDaoObj.getScenePic(args.second.toString().toStdString(),errorStr);
        if(errorStr.empty()){
            QImage img;
            img.loadFromData(QByteArray::fromStdString(imgStr));
            emit sigSceneImage(img);
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
#endif
    }else if(argType == GetStatis){
        std::vector<StatisTask> resVec = thriftDaoObj.getStatisInfo(errorStr);
        if(errorStr.empty()){
            emit sigStatisInfo(QVector<StatisTask>::fromStdVector(resVec));
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
    }else if(argType == AddStatis){
        QPair<QString,QString> arguments = args.second.value<QPair<QString,QString>>();
        bool res = thriftDaoObj.addStatis(arguments.first.toStdString(),arguments.second.toStdString(),errorStr);
        if(errorStr.empty()){
            emit sigResultState(res);
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
    }else if(argType == RemoveStatis){
        QPair<QString,QString> arguments = args.second.value<QPair<QString,QString>>();
        bool res = thriftDaoObj.removeStatis(arguments.first.toStdString(),arguments.second.toStdString(),errorStr);
        if(errorStr.empty()){
            emit sigResultState(res);
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
    }else if(argType == SetWaringArea){
        QPair<QString,QVector<QPair<int, QPolygonF>>> arguments = args.second.value<QPair<QString,QVector<QPair<int, QPolygonF>>>>();
        std::vector<std::pair<AreaType::type,std::vector<Point>>> polyPoints;
        if(!arguments.second.isEmpty()){
            arguments.second.remove(1,arguments.second.count() - 1);
            for(QPair<int, QPolygonF> &polygon : arguments.second) {
                std::vector<Point> pVec;
                for(QPointF &pf : polygon.second) {
                    Point cp;
                    cp.x = static_cast<int>(pf.x());
                    cp.y = static_cast<int>(pf.y());
                    pVec.push_back(cp);
                }
                polyPoints.push_back(std::make_pair(AreaType::type(polygon.first),pVec));
            }
            bool res = thriftDaoObj.setWaringArea(arguments.first.toStdString(),polyPoints.front().first,polyPoints.front().second,errorStr);
            if(errorStr.empty()){
                emit sigResultState(res);
            }else{
                emit sigError(QString::fromStdString(errorStr));
            }
        }else{
            bool res = thriftDaoObj.setWaringArea(arguments.first.toStdString(),AreaType::FORBIDDENZONE,std::vector<Point>(),errorStr);
            if(errorStr.empty()){
                emit sigResultState(res);
            }else{
                emit sigError(QString::fromStdString(errorStr));
            }
        }
    }else if(argType == GetWaringArea){
        Area waringArea = thriftDaoObj.getWaringArea(args.second.toString().toStdString(),errorStr);
        if(errorStr.empty()){
            QVector<QPair<int,QPolygonF>> polygons;
            QPolygonF polygon;
            std::transform(waringArea.points.begin(),waringArea.points.end(),std::back_inserter(polygon),[](const Point &p){return QPointF(p.x,p.y);});
            polygons << qMakePair(AreaType::type(waringArea.area_type),polygon);
            emit sigWaringAreas(polygons);
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
    }else if(argType == GetAlarmHistory){
        alarmHistoryArg alarmArgs = args.second.value<alarmHistoryArg>();
        PagedAlarmHis resData = thriftDaoObj.searchAlarmHistory(alarmArgs.page,alarmArgs.pageCount,alarmArgs.cameraId.toStdString(),alarmArgs.alarmType.toStdString(),
                                                                alarmArgs.start.toMSecsSinceEpoch(),alarmArgs.end.toMSecsSinceEpoch(),errorStr);
        if(errorStr.empty()){
            emit sigAlarmHistory(resData);
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
    }else if(argType == GetAlarmScene){
        std::string resPic = thriftDaoObj.getAlarmScenePic(args.second.toString().toStdString(),errorStr);
        if(errorStr.empty()){
            QImage img;
            img.loadFromData(QByteArray::fromStdString(resPic));
            emit sigSceneImage(img);
        }else{
            emit sigError(QString::fromStdString(errorStr));
        }
    }else if(argType == Login){
        LoginParameter loginArgs = args.second.value<LoginParameter>();
        bool result;
        HMODULE module = LoadLibrary(L"ServerCommandServiceClient");
        if(module == nullptr){
            std::wcerr << "LoadLibrary failed with error code " << GetLastError() << std::endl;
            emit sigResultState(false);
            return;
        }

        NmServerCommandService::CreateInstanceFuncPtr_t createInstanceFuncPtr = reinterpret_cast<NmServerCommandService::CreateInstanceFuncPtr_t>(GetProcAddress(module, "CreateInstance"));
        NmServerCommandService::DeleteInstanceFuncPtr_t deleteInstanceFuncPtr = reinterpret_cast<NmServerCommandService::DeleteInstanceFuncPtr_t>(GetProcAddress(module, "DeleteInstance"));
        if (createInstanceFuncPtr == NULL)
        {
            std::wcerr << "GetProcAddress(CreateInstance) failed with error code " << GetLastError() << std::endl;
            emit sigResultState(false);
            return;
        }
        else if (deleteInstanceFuncPtr == NULL)
        {
            std::wcerr << "GetProcAddress(DeleteInstance) failed with error code " << GetLastError() << std::endl;
            emit sigResultState(false);
            return;
        }
        // Create client instance
        NmServerCommandService::ImServerCommandServiceClient *client = NULL;
        client = createInstanceFuncPtr();

        // Setup connect details of the server command service
        client->SetServerHostName(loginArgs.ip.toLatin1().constData(), loginArgs.ip.length());
        client->SetServerPort(loginArgs.port);
        client->SetUserName(loginArgs.userName.toLatin1().constData(), loginArgs.userName.length());
        client->SetPassword(loginArgs.password.toLatin1().constData(), loginArgs.password.length());
        client->SetAuthenticationMethod(NmServerCommandService::AuthenticationMethod_t(loginArgs.authenticationMethod));
        client->SetServerProduct(NmServerCommandService::XProtectProductFamily_t(loginArgs.xprotectProductFamily));

        NmServerCommandService::utf8_char_t* tokenPtr = NULL;
        size_t tokenLength = 0;
        NmServerCommandService::utc_time_t registrationTime = 0;
        NmServerCommandService::time_span_t timeToLive = 0;
        bool limited = false;

        result = client->Login(&tokenPtr,&tokenLength,&registrationTime,&timeToLive,&limited);
        if (!result)
        {
            // The URL or Username/Password
            std::wcerr << "Unable to login to server" << std::endl;
            std::getchar();
            emit sigResultState(false);
            return;
        }

        const string token(tokenPtr, tokenLength);
        qDebug() << QString::fromStdString(token);
        std::wcerr << "... Login Completed " << std::endl;
        MileStoneSharedData::instance()->setClient(client);
        MileStoneSharedData::instance()->setToken(token);
        MileStoneSharedData::instance()->setVmsRecorderUri(loginArgs.host.toStdString());
        emit sigResultState(true);
    }else if(argType == GetPersonStayNumber){
        DLL::CloudHttpDao httpDao;
        int personNum = 0,time = 0;
        QString qerrorStr = httpDao.getPersonNumbers(args.second.value<PersonsStayArgs>(),personNum,time);
        if(qerrorStr.isEmpty()){
            emit sigPersonNumbers(personNum,time);
        }
    }else if(argType == GetPersonAverageTime){
        DLL::CloudHttpDao httpDao;
        QString qerrorStr = httpDao.getPeronAverageTime(args.second.value<AveragePersonTimeArgs>());
        if(qerrorStr.isEmpty()){
//            emit sigCameraGroup(resGroup);
        }
    }else if(argType == SemanticSearch){
        DLL::CloudHttpDao httpDao;
        SemanticReturnData resDatas;
        QString qerrorStr = httpDao.semanticSearch(args.second.value<SemanticSearchArgs>(),resDatas);
        if(qerrorStr.isEmpty()){
            emit sigSemanticSearch(resDatas);
        }else{
            emit sigError(qerrorStr);
        }
    }else if(argType == SearchUseImage){
        DLL::CloudHttpDao httpDao;
        QVector<RestServiceI::DataRectureItem> resVec;
        QString qerrorStr = httpDao.searchByImage(args.second.value<SearchUseImageArgs>(),resVec);
        if(qerrorStr.isEmpty()){
            emit sigSearchByImage(resVec);
        }else{
            emit sigError(qerrorStr);
        }
    }else if(argType == MultipleSearch){
        DLL::CloudHttpDao httpDao;
        QVector<RestServiceI::MultipleSearchItem> resData;
        QString qerrorStr = httpDao.multipleSearch(args.second.value<MultipleSearchArgs>(),resData);
        if(qerrorStr.isEmpty()){
            emit sigMultipleSearch(resData);
        }else{
            emit sigError(qerrorStr);
        }
    }else if(argType == SearchABDoorTime){

    }
}
