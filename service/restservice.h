#ifndef RESTSERVICE_H
#define RESTSERVICE_H
#include "core/baseworker.h"
#include "restservicei.h"

namespace BLL{
class RestService : public RestServiceI, public BaseWorker
{
    Q_OBJECT
public:
    struct alarmHistoryArg
    {
        int page;
        int pageCount;
        QString cameraId;
        QString alarmType;
        QDateTime start;
        QDateTime end;
    };
    RestService(WorkerManager *wm, QObject*parent = nullptr);
    WorkerType workerType() const override;
    void login(const LoginParameter &args) override;
    void combinationSearch(CombinationSearchArgs &) override;
    void getSceneInfo(const QString old) override;
    void getFaceLinkDataColl(FaceLinkDataCollArgs &args) override;
    void faceTracking(FaceTrackingArgs) override;
    void searchAbDoorTime(SearchABDoorTimeArg &) override;
    void multipleSearch(MultipleSearchArgs &) override;
    void getPersonDetails(QString &) override;
    void getAlarmScenePic(const QString oid) override;
    void getImageByUrl(QString &) override;
    void getFaceLinkTree(QString &) override;
    void getPersonStayTotalCount(PersonsStayArgs &) override;
    void getAverageTime(AveragePersonTimeArgs &) override;
    void getTop(const int id) override;
    void getCameraInfo() override;
    void generateFaceLink(FaceLinkArgs) override;
    void getFaceLinkPoint(QString &) override;
    void getCameraGroup(QString gropuNo) override;
    void getCameraDevice(QString groupNo) override;
    void addStatis(const QString startId,const QString endId) override;
    void removeStatis(const QString startId,const QString endId) override;
    void getStatisInfo() override;
    void captureSearch(CaptureSearchArgs &) override;
    void setWaringArea(const QString cameraId,const QVector<QPair<int,QPolygonF>> &polygons) override;
    void getWaringArea(const QString) override;
    void searchAlarmHistory(const int page,const int pageCount, const QString &cameraId,const QString &alarmType,const QDateTime &start,const QDateTime &end) override;

    void semanticSearch(SemanticSearchArgs &);
    void searchByImage(SearchUseImageArgs &);

protected:
    void run() override;
};
}

Q_DECLARE_METATYPE(BLL::RestService::alarmHistoryArg)
#endif // RESTSERVICE_H
