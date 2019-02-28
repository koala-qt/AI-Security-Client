#ifndef RESTSERVICE_H
#define RESTSERVICE_H
#include "servicei.h"
#include "dao/thriftdao.h"
#include "dao/cloudhttpdao.h"

class RestConcurrent : public RestServiceI
{
    Q_OBJECT
public:
    RestConcurrent(QObject*parent = nullptr);
    ~RestConcurrent();
    void combinationSearch(CombinationSearchArgs &) override;
    void getSceneInfo(const QString old) override;
    void getFaceLinkDataColl(FaceLinkDataCollArgs &args) override;
    void faceTracking(FaceTrackingArgs) override;
    void multipleSearch(MultipleSearchArgs &) override;
    void getPersonDetails(QString &) override;
    void getAlarmScenePic(const QString oid) override;
    void getImageByUrl(QString &) override;
    void getImagesByUrlList(QStringList &);
    void getFaceLinkTree(QString &) override;
    void getCameraInfo() override;
    void generateFaceLink(FaceLinkArgs) override;
    void getFaceLinkPoint(QString &) override;
    void getCameraGroup(QString gropuNo) override;
    void getCameraDevice(QString groupNo) override;
    void addStatis(const QString startId,const QString endId) override;
    void removeStatis(const QString startId,const QString endId) override;
    void getStatisInfo() override;
    void captureSearch(CaptureSearchArgs &) override;
    void setWaringArea(const QString cameraId, const QVector<QPair<int,QPolygonF>> &arguments) override;
    void getWaringArea(const QString) override;
    void searchAlarmHistory(EventSearchArgs &) override;
    void semanticSearch(SemanticSearchArgs &) override;
    void searchByImage(SearchUseImageArgs &) override;
    void uploadVideo(QString videoPath) override;

private:
    ThriftDao thriftDao_;
    DLL::CloudHttpDao *curlRest_{nullptr};
};

#endif // RESTSERVICE_H