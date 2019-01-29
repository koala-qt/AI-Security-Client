#ifndef CLOUDHTTPDAO_H
#define CLOUDHTTPDAO_H
#include "core/httpexector.h"
#include "service/restservicei.h"
namespace DLL{
class CloudHttpDao : public HttpExector
{
public:
    CloudHttpDao();
    QString getGroup(QString groupNo,QVector<RestServiceI::CameraGoup> &resGroups);
    QString getDevice(QString groupNo,QVector<CameraInfo> &devices);
    QString faceLink(RestServiceI::FaceLinkArgs &args);
    QString getFaceLinkPoint(QString &faceLinkId,RestServiceI::FaceLinkPointData &rootPointData);
    QString getFaceLinkTree(QString &objectId,QJsonObject &treeJson);
    QString getPersonNumbers(RestServiceI::PersonsStayArgs &args,int &num);
    QString getPeronAverageTime(RestServiceI::AveragePersonTimeArgs &args);
    QString getPersonDetailes(QString &objId);

    QString semanticSearch(RestServiceI::SemanticSearchArgs &);
    QString searchByImage(RestServiceI::SearchUseImageArgs &);

private:
    QString host_;
};
}

#endif // CLOUDHTTPDAO_H
