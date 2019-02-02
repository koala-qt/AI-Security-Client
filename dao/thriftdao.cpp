#include <QSettings>
#include "thriftdao.h"

ThriftDao::ThriftDao()
{
    QSettings settings("config.ini",QSettings::IniFormat);
    TSocket *tcpsocket = new TSocket(settings.value("Thrift/ip").toString().toStdString(),settings.value("Thrift/port").toInt());
    stdcxx::shared_ptr<TTransport> socket(tcpsocket);
    transport_ = stdcxx::shared_ptr<TBufferedTransport>(new TBufferedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport_));
    tcpsocket->setConnTimeout(1500);
    //tcpsocket->setRecvTimeout(2000);
    tcpsocket->setSendTimeout(2000);
    client_ = new FaceServClient(protocol);
}

ThriftDao::~ThriftDao()
{
    transport_->close();
    delete client_;
}

string ThriftDao::getScenePic(const string old, string &errorStr)
{
    std::string img;
    try{
        transport_->open();
        ScenePic res;
        client_->getScenePic(res,old);
        img = res.scene_img;
    }catch(TException &e){
        errorStr = e.what();
    }
    return img;
}

string ThriftDao::getAlarmScenePic(const string oid, string &errorStr)
{
    std::string img;
    try{
        transport_->open();
        ScenePic res;
        client_->getAlarmScenePic(res,oid);
        img = res.scene_img;
    }catch(TException &e){
        errorStr = e.what();
    }
    return img;
}

void ThriftDao::getTop(const int id)
{

}

bool ThriftDao::addStatis(const string startId, const string endId, string &errorStr)
{
    bool isOk = false;
    try{
        transport_->open();
        isOk = client_->addStatisTask(startId,endId);
    }catch(TException &e){
        errorStr = e.what();
    }
    return isOk;
}

bool ThriftDao::removeStatis(const string startId, const string endId, string &errorStr)
{
    bool isOk = false;
    try{
        transport_->open();
        isOk = client_->deleteStatisTask(startId,errorStr);
    }catch(TException &e){
        errorStr = e.what();
    }
    return isOk;
}

std::vector<StatisTask> ThriftDao::getStatisInfo(string &errorStr)
{
    std::vector<StatisTask> statisInfoVec;
    try{
        transport_->open();
        client_->getStatisTask(statisInfoVec);
    }catch(TException &e){
        errorStr = e.what();
    }
    return statisInfoVec;
}

PagedSnapFaceHis ThriftDao::captureSearch(const int page, const int pageCount, const string &position, const unsigned long long startTimespec, const unsigned long long endTimespec, string &errorStr)
{
    PagedSnapFaceHis resInfo;
    try{
        transport_->open();
        client_->pagenateSnapHistory(resInfo,page,pageCount,position,startTimespec,endTimespec);
    }catch(TException &e){
        errorStr = e.what();
    }
    return resInfo;
}

bool ThriftDao::setWaringArea(const string cameraId, AreaType::type t, std::vector<Point> &area, string &errorStr)
{
    bool isOk = false;
    try{
        transport_->open();
        isOk = client_->setAlarmArea(cameraId,t,area);
    }catch(TException &e){
        errorStr = e.what();
    }
    return isOk;
}

Area ThriftDao::getWaringArea(const string &cameraId, string &errorStr)
{
    Area waringArea;
    try{
        transport_->open();
        client_->getAlarmArea(waringArea,cameraId);
    }catch(TException &e){
        errorStr = e.what();
    }
    return waringArea;
}

PagedAlarmHis ThriftDao::searchAlarmHistory(const int page, const int pageCount, const string &cameraId, const string &alarmType, const unsigned long long &startTimestamp, const unsigned long long &endTimestamp, string &errorStr)
{
    PagedAlarmHis resInfo;
    try{
        transport_->open();
        client_->pagenateAlarmHistory(resInfo,page,pageCount,cameraId,alarmType,startTimestamp,endTimestamp);
    }catch(TException &e){
        errorStr = e.what();
    }
    return resInfo;
}

std::vector<SearchFace> ThriftDao::searchSnap(const string &img, const string &oid, const string &cameraId, const int topK, double similarty, unsigned long long startTimestamp, unsigned long long endTimestamp, string &errorStr)
{
    std::vector<SearchFace> resInfo;
    try{
        transport_->open();
        client_->searchSnap(resInfo,oid,img,cameraId,topK,similarty,startTimestamp,endTimestamp);
    }catch(TException &e){
        errorStr = e.what();
    }
    return resInfo;
}

std::vector<SearchFace> ThriftDao::generateTrackingByFace(string &oid, string &face_img, float thresh, unsigned long long startTime, unsigned long long endTime, string &erroStr)
{
    std::vector<SearchFace> resVec;
    try{
        transport_->open();
        client_->gnerateFaceTrace(resVec,oid,face_img,thresh,startTime,endTime);
    }catch(TException &e){
        erroStr = e.what();
    }
    return resVec;
}
