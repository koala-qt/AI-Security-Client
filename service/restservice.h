#ifndef RESTSERVICE_H
#define RESTSERVICE_H
#include "core/baseworker.h"
#include "restservicei.h"

namespace BLL{
class RestService : public RestServiceI, public BaseWorker
{
    Q_OBJECT
public:
    struct capSearchArg
    {
        QString dataBaseName;
        int page;
        int pageCount;
        QString position;
        QDateTime start;
        QDateTime end;
    };
    struct alarmHistoryArg
    {
        int page;
        int pageCount;
        QString cameraId;
        QString alarmType;
        QDateTime start;
        QDateTime end;
    };
    struct searchSnapArgs{
        QString dataBaseName;
        QImage img;
        QString oid;
        QString cameraId;
        int topK;
        double similarty;
        QDateTime start;
        QDateTime end;
    };
    RestService(WorkerManager *wm, QObject*parent = nullptr);
    WorkerType workerType() const override;
    void login(const LoginParameter &args) override;
    void combinationSearch(CombinationSearchArgs &) override;
    void getScenePic(const QString old) override;
    void faceTracking(FaceTrackingArgs) override;
    void searchAbDoorTime(SearchABDoorTimeArg &) override;
    void multipleSearch(MultipleSearchArgs &) override;
    void getPersonDetails(QString &) override;
    void getAlarmScenePic(const QString oid) override;
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
    void captureSearch(QString &dataBasename,const int page,const int pageCount, const QString &position, const QDateTime &start, const QDateTime &end) override;
    void setWaringArea(const QString cameraId,const QVector<QPair<int,QPolygonF>> &polygons) override;
    void getWaringArea(const QString) override;
    void searchAlarmHistory(const int page,const int pageCount, const QString &cameraId,const QString &alarmType,const QDateTime &start,const QDateTime &end) override;
    void searchSnap(const QString &dataBasename,const QImage &img,const QString &oid,const QString &cameraId,const int topK,double similarty,QDateTime &start,QDateTime &end) override;

    void semanticSearch(SemanticSearchArgs &);
    void searchByImage(SearchUseImageArgs &);

protected:
    void run() override;
};
}

Q_DECLARE_METATYPE(BLL::RestService::capSearchArg)
Q_DECLARE_METATYPE(BLL::RestService::alarmHistoryArg)
Q_DECLARE_METATYPE(BLL::RestService::searchSnapArgs)
#endif // RESTSERVICE_H
