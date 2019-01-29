#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QSettings>
#include <QUrl>
#include <QBuffer>
#include <QDebug>
#include "cloudhttpdao.h"

DLL::CloudHttpDao::CloudHttpDao()
{
    std::vector<std::string> headers;
    headers.emplace_back("Content-Type:application/json;charset=UTF-8");
    headers.emplace_back("User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    headers.emplace_back("Expect:");
    setheader(headers);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    host_ = configSetting.value("CloudHost/host").toString();
}

QString DLL::CloudHttpDao::getGroup(QString groupNo, QVector<RestServiceI::CameraGoup> &resGroups)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/device/group/find?groupNo=%1").arg(groupNo);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    QJsonObject jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    QJsonArray jsArray = jsObj.value("data").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resGroups),[](const QJsonValue &jsVal){
        RestServiceI::CameraGoup caGroup;
        QJsonObject caGroupObj = jsVal.toObject();
        caGroup.description = caGroupObj.value("description").toString();
        caGroup.groupNo = caGroupObj.value("groupNo").toString();
        caGroup.groupName = caGroupObj.value("groupName").toString();
        caGroup.deviceNumber = caGroupObj.value("deviceNumber").toInt();
        return caGroup;
    });
    return QString();
}

QString DLL::CloudHttpDao::getDevice(QString groupNo, QVector<CameraInfo> &devices)
{
    QString urlStr = host_ + QObject::tr("api/v2/external/device/info/find?type=1&pageSize=10000000&pageNo=1&groupNo=%1").arg(groupNo);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    QJsonObject jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    QJsonArray jsArray = jsObj.value("data").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(devices),[](const QJsonValue &jsVal){
        CameraInfo camera;
        QJsonObject cameraObj = jsVal.toObject();
        QString rtspStr = cameraObj.value("sourceAddress").toString();
        camera.rtsp = rtspStr.toStdString();
        int spiteIndex = rtspStr.lastIndexOf('/');
        camera.id = rtspStr.mid(spiteIndex+1).toStdString();
        camera.position = cameraObj.value("name").toString().toStdString();
        return camera;
    });
    return QString();
}

QString DLL::CloudHttpDao::faceLink(RestServiceI::FaceLinkArgs &args)
{
    QString urlStr = host_ + QObject::tr("graph/node/build/link");
    QByteArray imgArray;
    QBuffer imgBuf(&imgArray);
    imgBuf.open(QIODevice::WriteOnly);
    args.faceImg.save(&imgBuf,"PNG");
    QJsonObject jsObj{{"start_time",args.startT.toMSecsSinceEpoch()},
                      {"end_time",args.endT.toMSecsSinceEpoch()},
                      {"depth",args.depth},
                      {"num",args.num},
                      {"oid",args.oid},
                      {"thresh",args.thresh},
                      {"file",QString::fromLatin1(imgArray.toBase64())}};
    QJsonDocument jsDoc(jsObj);
    QByteArray argsJsonArray = jsDoc.toJson();
    int resCode = send(DLL::POST,urlStr.toStdString(),argsJsonArray.toStdString(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    qDebug() << QString::fromStdString(responseData());
    return QString();
}

QString DLL::CloudHttpDao::getFaceLinkPoint(QString &faceLinkId, RestServiceI::FaceLinkPointData &rootPointData)
{
    QString urlStr = host_ + QObject::tr("graph/node/find/name?name=%1").arg(faceLinkId);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    QJsonObject jsObj = jsDoc.object();
    QString status = jsObj.value("status").toString();
    QString message = jsObj.value("message").toString();
    QJsonObject rootObj = jsObj.value("data").toObject();
    rootPointData.id = rootObj.value("id").toInt();
    rootPointData.name = rootObj.value("name").toString();
    rootPointData.imgUrl = rootObj.value("image").toString();
    QJsonArray childArray = rootObj.value("faceNodePros").toArray();
    QVector<RestServiceI::FaceLinkPointData> childVec;
    std::transform(childArray.begin(),childArray.end(),std::back_inserter(childVec),[](QJsonValue val){
        RestServiceI::FaceLinkPointData pointData;
        QJsonObject subObj = val.toObject();
        pointData.nodeProId = subObj.value("nodeProId").toInt();
        pointData.intimacy = subObj.value("intimacy").toString();
        subObj = subObj.value("endNode").toObject();
        pointData.id = subObj.value("id").toInt();
        pointData.name = subObj.value("name").toString();
        pointData.imgUrl = subObj.value("image").toString();
        return pointData;
    });
    rootPointData.childrenPoint = childVec;
    return QString();
}

QString DLL::CloudHttpDao::getFaceLinkTree(QString &objectId, QJsonObject &treeJson)
{
    QString urlStr = host_ + QObject::tr("graph/node/find/hierarchical?objId=%1").arg(objectId);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    QJsonObject jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    QJsonArray resArray = jsObj.value("data").toArray();
    if(!resArray.isEmpty()){
        treeJson = resArray.first().toObject();
    }
    return QString();
}

QString DLL::CloudHttpDao::getPersonNumbers(RestServiceI::PersonsStayArgs &args,int &num)
{
    QString urlStr = host_ + QObject::tr("api/v2/external/monitor-detail/person-total?cameraId=%1&startTime=%2&finishTime=%3")
            .arg(args.cameraId)
            .arg(args.startT.toString("yyyy-MM-dd%20HH:mm:ss"))
            .arg(args.endT.toString("yyyy-MM-dd%20HH:mm:ss"));
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    QJsonObject jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    num = jsObj.value("data").toInt();
    return QString();
}

QString DLL::CloudHttpDao::getPeronAverageTime(RestServiceI::AveragePersonTimeArgs &args)
{
    QString urlStr = host_ + QObject::tr("graph/node/find/hierarchical?objId=%1");
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    QJsonObject jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    return QString();
}
