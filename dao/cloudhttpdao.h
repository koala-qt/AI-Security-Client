#ifndef CLOUDHTTPDAO_H
#define CLOUDHTTPDAO_H
#include "core/httpexector.h"
#include "service/restservicei.h"
namespace DLL{
class CloudHttpDao : public HttpExector
{
public:
    CloudHttpDao();
    QString getCameraInfoMap(QMap<QString, QVariant> &cameraMap);
    QString getGroup(QString groupNo,QVector<RestServiceI::CameraGoup> &resGroups);
    QString getDevice(QString groupNo,QVector<CameraInfo> &devices);
    QString faceLink(RestServiceI::FaceLinkArgs &args);
    QString faceLink_(RestServiceI::FaceLinkArgs &args,QString &finishId);
    QString getFaceLinkPoint(QString &faceLinkId,RestServiceI::FaceLinkPointData &rootPointData);
    QString getFaceLinkTree(QString &objectId,QJsonObject &treeJson);
    QString tracking(RestServiceI::FaceTrackingArgs &args, QVector<RestServiceI::TrackingReturnData> &resVec);
    QString getPersonNumbers(RestServiceI::PersonsStayArgs &args,int &num);
    QString getPeronAverageTime(RestServiceI::AveragePersonTimeArgs &args);
    QString getPersonDetailes(QString &objId,QImage &face,QImage &body,QStringList &attrsface,QStringList &attrsbody);
    QString getScenePic(QString &scenId,QImage &img);

    QString semanticSearch(RestServiceI::SemanticSearchArgs &,RestServiceI::SemanticReturnData &resDatas);
    QString searchByImage(RestServiceI::SearchUseImageArgs &, QVector<RestServiceI::DataRectureItem> &resVec);
    QString multipleSearch(RestServiceI::MultipleSearchArgs &);

private:
    QString host_;
    float attributeThresold_ = 0.5;
};
}

#endif // CLOUDHTTPDAO_H
