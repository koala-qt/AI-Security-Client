#include <QBuffer>
#include <QSettings>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QDebug>
#include "restserviceconcureent.h"
#include "dao/cpphttpdao.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

RestConcurrent::RestConcurrent(QObject *parent):
    RestServiceI(parent)
{
    curlRest_ = new DLL::CloudHttpDao;
}

RestConcurrent::~RestConcurrent()
{
    delete curlRest_;
    qDebug() << metaObject()->className() << "deleted";
}

void RestConcurrent::combinationSearch(CombinationSearchArgs &args)
{
    CombinationSearchReturenData *resData = new CombinationSearchReturenData;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resData,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigCombinationSearch(*resData);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resData;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::combinationSearch,args,resData));
}

void RestConcurrent::getSceneInfo(const QString old)
{
    SceneInfo *sceneInfo = new SceneInfo;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,sceneInfo,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigSceneInfo(*sceneInfo);
        }else{
            emit sigError(fwatcher->result());
        }
        delete sceneInfo;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getSceneInfo,old,sceneInfo));
}

void RestConcurrent::getFaceLinkDataColl(FaceLinkDataCollArgs &args)
{
    FaceLinkDataCollReturn *resData = new FaceLinkDataCollReturn;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resData,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigFaceLinkDataColl(*resData);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resData;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getFaceLinkDataColl,args,resData));
}

void RestConcurrent::faceTracking(FaceTrackingArgs args)
{
    QVector<TrackingReturnData> *resVec = new QVector<TrackingReturnData>();
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resVec,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigTrackingNew(*resVec);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resVec;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::tracking,args,resVec));
}

void RestConcurrent::multipleSearch(MultipleSearchArgs &args)
{
    QVector<MultipleSearchItem> *resData = new QVector<MultipleSearchItem>();
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resData,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigMultipleSearch(*resData);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resData;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::multipleSearch,args,resData));
}

void RestConcurrent::getPersonDetails(QString &objId)
{
    PortraitReturnData *resDatas = new PortraitReturnData;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[=]{
        if(fwatcher->result().isEmpty()){
            emit sigPeronsDetails(*resDatas);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resDatas;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getPersonDetailes,objId,resDatas));
}

void RestConcurrent::getAlarmScenePic(const QString oid)
{
    std::string *errorStr = new std::string;
    QFutureWatcher<std::string> *fwatcher = new QFutureWatcher<std::string>(this);
    connect(fwatcher,&QFutureWatcher<std::string>::finished,this,[this,errorStr,fwatcher]{
        if(errorStr->empty()){
            QImage img;
            img.loadFromData(QByteArray::fromStdString(fwatcher->result()));
            emit sigDownloadImage(img);
        }else{
            emit sigError(QString::fromStdString(*errorStr));
        }
        delete errorStr;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::getAlarmScenePic,oid.toStdString(),errorStr));
}

void RestConcurrent::getImageByUrl(QString &url){
    QImage *img = new QImage;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,img,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigDownloadImage(*img);
        }else{
            emit sigError(fwatcher->result());
        }
        delete img;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getImageByUrl,url,img));
}

void RestConcurrent::getImagesByUrlList(QStringList &urlList)
{
    QFutureWatcher<QImage> *fwatcher = new QFutureWatcher<QImage>(this);
    connect(fwatcher,&QFutureWatcher<QImage>::finished,this,[this,fwatcher]{
        emit sigDownloadImages(QVector<QImage>::fromList(fwatcher->future().results()));
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    std::function<QImage(QString)> f = [this](QString url)->QImage{return curlRest_->getImageByUrl(url);};
    fwatcher->setFuture(QtConcurrent::mapped(urlList,f));
}

void RestConcurrent::getFaceLinkTree(QString &objectID)
{
    QJsonObject *jsObj = new QJsonObject;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,jsObj,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigFaceLinkTree(*jsObj);
        }
        delete jsObj;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getFaceLinkTree,objectID,jsObj));
}

void RestConcurrent::getCameraInfo()
{
    QVector<CameraInfo> *cameras = new QVector<CameraInfo>();
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,cameras,fwatcher]{
        if(fwatcher->result().isEmpty()){
            qDebug() << cameras->count() << "---------";
            emit sigCameraInfo(*cameras);
        }
        delete cameras;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getCameraInfo,cameras));
}

void RestConcurrent::generateFaceLink(FaceLinkArgs args)
{
    QString *finishId = new QString;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,finishId,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigFaceLinkFinished(*finishId);
        }else{
            emit sigError(fwatcher->result());
        }
        delete finishId;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::faceLink_,args,finishId));
}

void RestConcurrent::getFaceLinkPoint(QString &faceLinkId)
{
    FaceLinkPointData *rootPointdata = new FaceLinkPointData;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,rootPointdata,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigFaceLinkData(*rootPointdata);
        }else{
            emit sigError(fwatcher->result());
        }
        delete rootPointdata;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getFaceLinkPoint,faceLinkId,rootPointdata));
}

void RestConcurrent::getCameraGroup(QString gropuNo)
{
    QVector<CameraGoup> *resGroup = new QVector<CameraGoup>();
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resGroup,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigCameraGroup(*resGroup);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resGroup;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getGroup,gropuNo,resGroup));
}

void RestConcurrent::getCameraDevice(QString groupNo)
{
    QVector<CameraInfo> *devices = new QVector<CameraInfo>();
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,devices,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigCameraInfo(*devices);
        }else{
            emit sigError(fwatcher->result());
        }
        delete devices;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::getDevice,groupNo,devices));
}

void RestConcurrent::addStatis(const QString startId, const QString endId)
{
    std::string *errorStr = new std::string;
    QFutureWatcher<bool> *fwatcher = new QFutureWatcher<bool>(this);
    connect(fwatcher,&QFutureWatcher<bool>::finished,this,[this,errorStr,fwatcher]{
        if(errorStr->empty()){
            emit sigResultState(fwatcher->result());
        }else{
            emit sigError(QString::fromStdString(*errorStr));
        }
        delete errorStr;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::addStatis,startId.toStdString(),endId.toStdString(),errorStr));
}

void RestConcurrent::removeStatis(const QString startId, const QString endId)
{
    std::string *errorStr = new std::string;
    QFutureWatcher<bool> *fwatcher = new QFutureWatcher<bool>(this);
    connect(fwatcher,&QFutureWatcher<bool>::finished,this,[this,errorStr,fwatcher]{
        if(errorStr->empty()){
            emit sigResultState(fwatcher->result());
        }else{
            emit sigError(QString::fromStdString(*errorStr));
        }
        delete errorStr;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::removeStatis,startId.toStdString(),endId.toStdString(),errorStr));
}

void RestConcurrent::getStatisInfo()
{
    std::string *errorStr = new std::string;
    QFutureWatcher<std::vector<StatisTask>> *fwatcher = new QFutureWatcher<std::vector<StatisTask>>(this);
    connect(fwatcher,&QFutureWatcher<std::vector<StatisTask>>::finished,this,[this,errorStr,fwatcher]{
        if(errorStr->empty()){
            emit sigStatisInfo(QVector<StatisTask>::fromStdVector(fwatcher->result()));
        }else{
            emit sigError(QString::fromStdString(*errorStr));
        }
        delete errorStr;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::getStatisInfo,errorStr));
}

void RestConcurrent::captureSearch(CaptureSearchArgs &args)
{
    CaptureSearchReturnData *resdatas = new CaptureSearchReturnData;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resdatas,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigCaptureSearch(*resdatas);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resdatas;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::captureSearch,args,resdatas));
}

void RestConcurrent::getWaringArea(const QString cameraId)
{
    std::string *errorStr = new std::string;
    QFutureWatcher<Area> *fwatcher = new QFutureWatcher<Area>(this);
    connect(fwatcher,&QFutureWatcher<Area>::finished,this,[this,errorStr,fwatcher]{
        if(errorStr->empty()){
            QVector<QPair<int,QPolygonF>> polygons;
            QPolygonF polygon;
            Area res;
            qSwap(res,fwatcher->result());
            std::transform(res.points.begin(),res.points.end(),std::back_inserter(polygon),[](const Point &p){return QPointF(p.x,p.y);});
            polygons << qMakePair(AreaType::type(res.area_type),polygon);
            emit sigWaringAreas(polygons);
        }else{
            emit sigError(QString::fromStdString(*errorStr));
        }
        delete errorStr;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::getWaringArea,cameraId.toStdString(),errorStr));
}

void RestConcurrent::searchAlarmHistory(EventSearchArgs &args)
{
#if 0
    std::string *errorStr = new std::string;
    QFutureWatcher<PagedAlarmHis> *fwatcher = new QFutureWatcher<PagedAlarmHis>(this);
    connect(fwatcher,&QFutureWatcher<PagedAlarmHis>::finished,this,[this,errorStr,fwatcher]{
        if(errorStr->empty()){
            emit sigAlarmHistory(fwatcher->result());
        }else{
            emit sigError(QString::fromStdString(*errorStr));
        }
        delete errorStr;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run([=]()->PagedAlarmHis{
        return thriftDao_.searchAlarmHistory(page,pageCount,cameraId.toStdString(),alarmType.toStdString(),start.toMSecsSinceEpoch(),end.toMSecsSinceEpoch(),errorStr);
    }));
#else
    EventSearchReturn *resDatas = new EventSearchReturn;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resDatas,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigAlarmHistory(*resDatas);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resDatas;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::eventSearch,args,resDatas));
#endif
}

void RestConcurrent::semanticSearch(SemanticSearchArgs &args)
{
    SemanticReturnData *resDatas = new SemanticReturnData;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resDatas,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigSemanticSearch(*resDatas);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resDatas;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::semanticSearch,args,resDatas));
}

void RestConcurrent::searchByImage(SearchUseImageArgs &args)
{
    QVector<DataRectureItem> *resVec = new QVector<DataRectureItem>;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resVec,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigSearchByImage(*resVec);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resVec;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::searchByImage,args,resVec));
}

void RestConcurrent::uploadVideo(QString videoPath)
{
    DLL::CppHttpDao *cppDao = new DLL::CppHttpDao;
    cppDao->setUploadProgressCallBack([this](double total,double uploaded)->void{
        QMetaObject::invokeMethod(this,"sigVideoUploadProgress",Qt::QueuedConnection,Q_ARG(double,total),Q_ARG(double,uploaded));
    });
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,fwatcher,cppDao]{
        if(fwatcher->result().isEmpty()){
            emit sigResultState(true);
        }else{
            emit sigError(fwatcher->result());
        }
        delete cppDao;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(cppDao,&DLL::CppHttpDao::uploadVideo,videoPath));
}

void RestConcurrent::getAvailabelAttrs(RestServiceI::SearchAttrsArgs & args)
{
    QStringList *resVec = new QStringList;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resVec,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigAvailableAttrs(*resVec);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resVec;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::searchAvailableAttribute,args,resVec));
}

void RestConcurrent::portraitLibCompSearch(RestServiceI::PortraitLibCompArgs &args)
{
    QVector<PortraitLibCompItem> *resVec = new QVector<PortraitLibCompItem>;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resVec,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigPortraitLibCompResult(*resVec);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resVec;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::portraitLibCompSearch,args,resVec));
}

void RestConcurrent::mnFaceAnalysisSearch(RestServiceI::MNFaceAnalysisArgs &args)
{
    QVector<MNFaceAnalysisItem> *resVec = new QVector<MNFaceAnalysisItem>;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resVec,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigMNFaceAnalysisResult(*resVec);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resVec;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::mnFaceAnalysisSearch,args,resVec));
}

void RestConcurrent::queryPersonTypes()
{
    QVector<PersonType> *resVec = new QVector<PersonType>;
    QFutureWatcher<QString> *fwatcher = new QFutureWatcher<QString>(this);
    connect(fwatcher,&QFutureWatcher<QString>::finished,this,[this,resVec,fwatcher]{
        if(fwatcher->result().isEmpty()){
            emit sigPersonTypesResult(*resVec);
        }else{
            emit sigError(fwatcher->result());
        }
        delete resVec;
    });
    connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
    fwatcher->setFuture(QtConcurrent::run(curlRest_,&DLL::CloudHttpDao::queryPersonTypes,resVec));
}

void RestConcurrent::setWaringArea(const QString cameraId, const QVector<QPair<int, QPolygonF> > &polygonsa)
{
    std::vector<std::pair<AreaType::type,std::vector<Point>>> polyPoints;
    if(!polygonsa.isEmpty()){
        QVector<QPair<int, QPolygonF> > polygons = polygonsa;
        polygons.remove(1,polygons.count() - 1);
        for(const QPair<int, QPolygonF> &polygon : polygons) {
            std::vector<Point> pVec;
            for(const QPointF &pf : polygon.second) {
                Point cp;
                cp.x = static_cast<int>(pf.x());
                cp.y = static_cast<int>(pf.y());
                pVec.push_back(cp);
            }
            polyPoints.push_back(std::make_pair(AreaType::type(polygon.first),pVec));
        }

        std::string *errorStr = new std::string;
        QFutureWatcher<bool> *fwatcher = new QFutureWatcher<bool>(this);
        connect(fwatcher,&QFutureWatcher<bool>::finished,this,[this,errorStr,fwatcher]{
            if(errorStr->empty()){
                emit sigResultState(fwatcher->result());
            }else{
                emit sigError(QString::fromStdString(*errorStr));
            }
            delete errorStr;
        });
        connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
        fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::setWaringArea,cameraId.toStdString(),polyPoints.front().first,polyPoints.front().second,errorStr));
    }else{
        std::string *errorStr = new std::string;
        QFutureWatcher<bool> *fwatcher = new QFutureWatcher<bool>(this);
        connect(fwatcher,&QFutureWatcher<bool>::finished,this,[this,errorStr,fwatcher]{
            if(errorStr->empty()){
                emit sigResultState(fwatcher->result());
            }else{
                emit sigError(QString::fromStdString(*errorStr));
            }
            delete errorStr;
        });
        connect(fwatcher,SIGNAL(finished()),this,SLOT(deleteLater()));
        fwatcher->setFuture(QtConcurrent::run(&thriftDao_,&ThriftDao::setWaringArea,cameraId.toStdString(),AreaType::FORBIDDENZONE,std::vector<Point>(),errorStr));
    }
}
