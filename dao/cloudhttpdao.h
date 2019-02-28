#ifndef CLOUDHTTPDAO_H
#define CLOUDHTTPDAO_H
#include "core/httpexector.h"
#include "service/servicei.h"
namespace DLL{
class CloudHttpDao : public HttpExector
{
public:
    CloudHttpDao();
    QString getCameraInfo(QVector<RestServiceI::CameraInfo> *cameras);
    QString getGroup(QString groupNo,QVector<RestServiceI::CameraGoup> *resGroups);
    QString getDevice(QString groupNo, QVector<RestServiceI::CameraInfo> *devices);
    QString faceLink(RestServiceI::FaceLinkArgs &args);
    QString faceLink_(RestServiceI::FaceLinkArgs &args,QString *finishId);
    QString getFaceLinkPoint(QString &faceLinkId,RestServiceI::FaceLinkPointData *rootPointData);
    QString getFaceLinkTree(QString &objectId,QJsonObject *treeJson);
    QString tracking(RestServiceI::FaceTrackingArgs &args, QVector<RestServiceI::TrackingReturnData> *resVec);
    QString getPersonNumbers(RestServiceI::PersonsStayArgs &args, int &num, int &times);
    QString getPeronAverageTime(RestServiceI::AveragePersonTimeArgs &args);
    QString getPersonDetailes(QString &objId,QImage *face,QImage *body,QStringList *attrsface,QStringList *attrsbody);
    QString getSceneInfo(QString &scenId,RestServiceI::SceneInfo *sceneInfo);
    QString getImageByUrl(QString &url,QImage *image);
    QImage getImageByUrl(QString url);
    QString captureSearch(RestServiceI::CaptureSearchArgs&, RestServiceI::CaptureSearchReturnData *resDatas);
    QString semanticSearch(RestServiceI::SemanticSearchArgs &,RestServiceI::SemanticReturnData *resDatas);
    QString searchByImage(RestServiceI::SearchUseImageArgs &, QVector<RestServiceI::DataRectureItem> *resVec);
    QString combinationSearch(RestServiceI::CombinationSearchArgs &, RestServiceI::CombinationSearchReturenData *resData);
    QString multipleSearch(RestServiceI::MultipleSearchArgs &,QVector<RestServiceI::MultipleSearchItem> *);
    QString getFaceLinkDataColl(RestServiceI::FaceLinkDataCollArgs &args, RestServiceI::FaceLinkDataCollReturn *resDatas);
    QString eventSearch(RestServiceI::EventSearchArgs &args,RestServiceI::EventSearchReturn *resData);
    /**
     * Added by aihc for Portrait library comparison.
     * @brief portraitLibCompSearch
     * @param resVec
     * @return
     */
    QString portraitLibCompSearch(RestServiceI::PortraitLibCompArgs &args, QVector<RestServiceI::PortraitLibCompItem> *resVec);
    // * Added by aihc for Portrait library comparison.
    QString queryPersonTypes(QVector<RestServiceI::PersonType> *resVec);

protected:
    int progress(double totalDownLoad,double downloaded,double totalUpload, double uploaded) override;

private:
    QString host_;
    float attributeThresold_ = 0.5;
};
}

#endif // CLOUDHTTPDAO_H
