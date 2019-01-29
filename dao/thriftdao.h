#ifndef THRIFTDAO_H
#define THRIFTDAO_H
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>
#include "FaceServ.h"
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class ThriftDao
{
public:
    ThriftDao();
    ~ThriftDao();
    std::string getScenePic(const std::string old,std::string &errorStr);
    std::string getAlarmScenePic(const std::string oid,std::string &errorStr);
    void getTop(const int id);
    std::vector<CameraInfo> getCameraInfo(std::string &errorStr);
    bool addStatis(const std::string startId,const std::string endId,std::string &errorStr);
    bool removeStatis(const std::string startId,const std::string endId,std::string &errorStr);
    std::vector<StatisTask> getStatisInfo(std::string &errorStr);
    PagedSnapFaceHis captureSearch(const int page,const int pageCount, const std::string &position, const unsigned long long startTimespec, const unsigned long long endTimespec,
                                   std::string &errorStr);
    bool setWaringArea(const std::string cameraId, AreaType::type t, std::vector<Point> &area, std::string &errorStr);
    Area getWaringArea(const std::string &cameraId,std::string &errorStr);
    PagedAlarmHis searchAlarmHistory(const int page,const int pageCount, const std::string &cameraId,const std::string &alarmType
                                     ,const unsigned long long &startTimestamp,const unsigned long long &endTimestamp,std::string &errorStr);
    std::vector<SearchFace> searchSnap(const std::string &img, const std::string &oid, const std::string &cameraId, const int topK, double similarty
                    , unsigned long long startTimestamp, unsigned long long endTimestamp, std::string &errorStr);
    std::vector<SearchFace> generateTrackingByFace(std::string &oid,std::string &face_img,float thresh,unsigned long long startTime,unsigned long long endTime,std::string &erroStr);

private:
    stdcxx::shared_ptr<TTransport> transport_;
    FaceServClient *client_{nullptr};
};

#endif // THRIFTDAO_H
