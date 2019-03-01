#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QSettings>
#include <QUrl>
#include <QBuffer>
#include <QRect>
#include <QDebug>
#include "cloudhttpdao.h"

DLL::CloudHttpDao::CloudHttpDao()
{
    std::vector<std::string> headers;
    headers.emplace_back("Accept-Language: en-US,en;q=0.9");
    headers.emplace_back("User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    headers.emplace_back("token:7d1e52d3cf0142e19b5901eb1ef91372");
    headers.emplace_back("Expect:");
    setheader(headers);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    host_ = configSetting.value("Http/Javahost").toString();
    attributeThresold_ = configSetting.value("Http/attributeThreshold").toFloat();

}

QString DLL::CloudHttpDao::getCameraInfo(QVector<RestServiceI::CameraInfo> *cameras)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/find-camera-map");
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
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(*cameras),[](QJsonValue jsVal){
        RestServiceI::CameraInfo camera;
        QJsonObject cameraObj = jsVal.toObject();
        camera.cameraId = QString::number(cameraObj.value("id").toInt());
        camera.cameraPos = cameraObj.value("pos").toString();
        camera.rtsp = cameraObj.value("client_rtsp").toString();
        return camera;
    });
    return QString();
}

QString DLL::CloudHttpDao::getGroup(QString groupNo, QVector<RestServiceI::CameraGoup> *resGroups)
{
    QString urlStr = host_ +  QObject::tr("api/v2/device/group/find?groupNo=%1").arg(groupNo);
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
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(*resGroups),[](const QJsonValue &jsVal){
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

QString DLL::CloudHttpDao::getDevice(QString groupNo, QVector<RestServiceI::CameraInfo> *devices)
{
    QString urlStr = host_ + QObject::tr("api/v2/device/info/find?type=1&pageSize=10000000&pageNo=1&groupNo=%1").arg(groupNo);
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
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(*devices),[](const QJsonValue &jsVal){
        RestServiceI::CameraInfo camera;
        QJsonObject cameraObj = jsVal.toObject();
        QString rtspStr = cameraObj.value("sourceAddress").toString();
        camera.rtsp = rtspStr;
        int spiteIndex = rtspStr.lastIndexOf('/');
        camera.cameraId = rtspStr.mid(spiteIndex+1);
        camera.cameraPos = cameraObj.value("name").toString();
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

QString DLL::CloudHttpDao::faceLink_(RestServiceI::FaceLinkArgs &args, QString *finishId)
{
    std::vector<std::string> headers;
    headers.emplace_back("Content-Type:application/json;charset=UTF-8");
    headers.emplace_back("Accept-Language: en-US,en;q=0.9");
    headers.emplace_back("User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    headers.emplace_back("token:7d1e52d3cf0142e19b5901eb1ef91372");
    headers.emplace_back("Expect:");
    setheader(headers);

    QByteArray imgArray;
    QBuffer imgBuf(&imgArray);
    imgBuf.open(QIODevice::WriteOnly);
    args.faceImg.save(&imgBuf,"jpg");
    QString urlStr = host_ + QObject::tr("api/v2/external/monitor-detail/person-link");
    qDebug() << urlStr << args.oid;
    QJsonObject jsObj{{"startTime",args.startT.toString("yyyy-MM-dd HH:mm:ss")},
                      {"finishTime",args.endT.toString("yyyy-MM-dd HH:mm:ss")},
                      {"depth",args.depth},
                      {"number",args.num},
                      {"objId",args.oid},
                      {"thresh",args.thresh},
                      {"base64",QString::fromLatin1(imgArray.toBase64(QByteArray::Base64UrlEncoding))}};
    QJsonDocument jsDoc(jsObj);
    QByteArray argsJsonArray = jsDoc.toJson();
    int resCode = send(DLL::POST,urlStr.toStdString(),argsJsonArray.toStdString(),30);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        return jsError.errorString();
    }

    jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    *finishId = jsObj.value("data").toString();
    return QString();
}

QString DLL::CloudHttpDao::getFaceLinkPoint(QString &faceLinkId, RestServiceI::FaceLinkPointData *rootPointData)
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
    rootPointData->id = rootObj.value("id").toInt();
    rootPointData->name = rootObj.value("name").toString();
    rootPointData->imgUrl = rootObj.value("image").toString();
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
    rootPointData->childrenPoint = childVec;
    return QString();
}

QString DLL::CloudHttpDao::getFaceLinkTree(QString &objectId, QJsonObject *treeJson)
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
        *treeJson = resArray.first().toObject();
    }
    return QString();
}

QString DLL::CloudHttpDao::tracking(RestServiceI::FaceTrackingArgs &args, QVector<RestServiceI::TrackingReturnData> *resVec)
{
    QByteArray imgArray;
    QBuffer imgBuf(&imgArray);
    imgBuf.open(QIODevice::WriteOnly);
    args.faceImg.save(&imgBuf,"jpg");
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/trajectory-tracking");
    QString postData = QObject::tr("base64=%1&objId=%2&similarity=%3&startTime=%4&finishTime=%5&property=false")
            .arg(QString(imgArray.toBase64(QByteArray::Base64UrlEncoding)),args.oid)
            .arg(args.thresh)
            .arg(args.startT.toString("yyyy-MM-dd HH:mm:ss"),args.endT.toString("yyyy-MM-dd HH:mm:ss"));
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),15);
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
#if 0
    for(auto &jsVal : jsArray){
        QJsonObject  dataObj = jsVal.toObject();
        RestServiceI::TrackingReturnData pdata;
        pdata.cameraId = dataObj.value("cameraId").toString();
        pdata.objId = dataObj.value("id").toString();
        pdata.faceImg.loadFromData(QByteArray::fromBase64(dataObj.value("faceSnap").toString().toLatin1()));
        pdata.timeIn = QDateTime::fromMSecsSinceEpoch(dataObj.value("tsIn").toVariant().toULongLong());
        pdata.timeOut = QDateTime::fromMSecsSinceEpoch(dataObj.value("tsOut").toVariant().toULongLong());
        resVec << pdata;
    }
#else 1
    QVector<RestServiceI::TrackingReturnData> allData;
    for(auto &jsVal : jsArray){
        QJsonObject  dataObj = jsVal.toObject();
        RestServiceI::TrackingReturnData pdata;
        pdata.cameraId = dataObj.value("cameraId").toString();
        pdata.objId = dataObj.value("id").toString();
        pdata.lat = dataObj.value("lat").toDouble();
        pdata.lng = dataObj.value("lng").toDouble();
        pdata.faceImg.loadFromData(QByteArray::fromBase64(dataObj.value("faceSnap").toString().toLatin1()));
        pdata.timeIn = QDateTime::fromMSecsSinceEpoch(dataObj.value("tsIn").toVariant().toULongLong());
        pdata.timeOut = QDateTime::fromMSecsSinceEpoch(dataObj.value("tsOut").toVariant().toULongLong());
        pdata.sceneId = dataObj.value("faceSceneId").toString();
        allData << pdata;
    }
    if(allData.isEmpty())return "no points";
    QVector<QVector<RestServiceI::TrackingReturnData>> rightDataVec;
    rightDataVec << (QVector<RestServiceI::TrackingReturnData>() << allData.first());
    for(int i = 1; i < allData.count(); i++){
        if(allData.at(i).cameraId != allData.at(i-1).cameraId){
            rightDataVec << (QVector<RestServiceI::TrackingReturnData>() << allData.at(i));
        }else{
            rightDataVec.last() << allData.at(i);
        }
    }
    for(QVector<RestServiceI::TrackingReturnData> &sameIdVec : rightDataVec){
        QVector<qint64> tsVec;
        for(RestServiceI::TrackingReturnData &ddv : sameIdVec){
            tsVec << ddv.timeIn.toMSecsSinceEpoch() << ddv.timeOut.toMSecsSinceEpoch();
        }
        auto minmaxRes = std::minmax_element(tsVec.begin(),tsVec.end());
        RestServiceI::TrackingReturnData rdata = sameIdVec.first();
        rdata.timeIn = QDateTime::fromMSecsSinceEpoch(*minmaxRes.first);
        rdata.timeOut = QDateTime::fromMSecsSinceEpoch(*minmaxRes.second);
        *resVec << rdata;
    }
#endif
    return QString();
}

QString DLL::CloudHttpDao::getPersonNumbers(RestServiceI::PersonsStayArgs &args,int &num,int &times)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/person-total?cameraId=%1&startTime=%2&finishTime=%3")
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
    QJsonObject dataObj = jsObj.value("data").toObject();
    num = dataObj.value("personNumber").toInt();
    times = dataObj.value("stayTime").toVariant().toLongLong();
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

QString DLL::CloudHttpDao::getPersonDetailes(QString &objId, RestServiceI::PortraitReturnData *resDatas)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/portrait?objId=%1&threshold=%2").arg(objId).arg(attributeThresold_);
    qDebug() << urlStr << objId;
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

    QJsonObject dataJsObj = jsObj.value("data").toObject();
    resDatas->faceImg.loadFromData(QByteArray::fromBase64(dataJsObj.value("faceSnap").toString().toLatin1()));
    resDatas->bodyImg.loadFromData(QByteArray::fromBase64(dataJsObj.value("bodySnap").toString().toLatin1()));
    resDatas->id = dataJsObj.value("personId").toString();
    resDatas->objId = dataJsObj.value("id").toString();
    resDatas->name = dataJsObj.value("personName").toString();
    resDatas->personType = dataJsObj.value("personTypeName").toString();
    QJsonArray jsArray = dataJsObj.value("faceAttrs").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resDatas->faceAttrs),[](QJsonValue jsValue){
        return jsValue.toString();
    });
    jsArray = dataJsObj.value("bodyAttrs").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resDatas->bodyAttrs),[](QJsonValue jsValue){
        return jsValue.toString();
    });
    return QString();
}

QString DLL::CloudHttpDao::getSceneInfo(QString &scenId, RestServiceI::SceneInfo *sceneInfo)
{
    QStringList requiredFileds;
    requiredFileds << "face_bbox" << "face_id" << "snapshot";
    QString urlStr = host_ + QObject::tr("api/v2/external/monitor-detail/find-document?collection=snap_scene&requiredFields=%1&objId=%2")
            .arg(requiredFileds.join(','))
            .arg(scenId);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),10);
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

    jsObj = jsObj.value("data").toObject();
    if(host_.isEmpty())return "host is empty";
    QString imgUrl = host_;
    imgUrl.remove(imgUrl.count() - 1, 1);
    imgUrl += jsObj.value("url").toString();
    QString errorStr = getImageByUrl(imgUrl,&sceneInfo->image);
    if(!errorStr.isEmpty()){
        return errorStr;
    }
    QVariantMap faceBoxMap = jsObj.value("faceBoxMapping").toObject().toVariantMap();
    QStringList mapKeys = faceBoxMap.keys();
    for(const QString &faceId : mapKeys){
        QVariantList pointsList = faceBoxMap.value(faceId).toList();
        if(pointsList.count() < 4)continue;
        qreal xPer = 0.5,yPer = 0.5;
        int x = pointsList.first().toInt() * xPer;
        int y = pointsList.at(1).toInt() * yPer;
        int rWidth = pointsList.at(2).toInt() * xPer;
        int rHeight = pointsList.at(3).toInt() * yPer;

        QImage faceImg;
        QString faceImgUrl = host_ + "api/v2/external/monitor-detail/download-image?collection=snap_face&fieldName=snapshot&objId=" + faceId;
        QString errorStr = getImageByUrl(faceImgUrl,&faceImg);
        if(!errorStr.isEmpty()){
            return errorStr;
        }
        sceneInfo->faceRectVec << qMakePair(QRect(x,y,rWidth,rHeight),faceImg);
    }
    sceneInfo->sceneId = scenId;
    return QString();
}

QString DLL::CloudHttpDao::getImageByUrl(QString &url, QImage *image)
{
    std::vector<std::string> headers;
    headers.emplace_back("Content-Type:application/json;charset=UTF-8");
    headers.emplace_back("User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    headers.emplace_back("token:7d1e52d3cf0142e19b5901eb1ef91372");
    headers.emplace_back("Expect:");
    setheader(headers);

    int resCode = send(DLL::GET,url.toStdString(),std::string(),10);
    if(resCode != CURLE_OK){
        return curl_easy_strerror(CURLcode(resCode));
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        image->loadFromData(QByteArray::fromStdString(responseData()));
        return QString();
    }

    QJsonObject jsObj = jsDoc.object();
    int status = jsObj.value("status").toInt();
    if(status != 200){
        return jsObj.value("message").toString();
    }
    return QString();
}

QImage DLL::CloudHttpDao::getImageByUrl(QString url)
{
    std::vector<std::string> headers;
    headers.emplace_back("Content-Type:application/json;charset=UTF-8");
    headers.emplace_back("User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    headers.emplace_back("token:7d1e52d3cf0142e19b5901eb1ef91372");
    headers.emplace_back("Expect:");
    setheader(headers);

    QImage image;
    send(DLL::GET,url.toStdString(),std::string(),2);
    image.loadFromData(QByteArray::fromStdString(responseData()));
    return image;
}

QString DLL::CloudHttpDao::captureSearch(RestServiceI::CaptureSearchArgs &args, RestServiceI::CaptureSearchReturnData *resDatas)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/find-history");
    QString postData = QObject::tr("mode=4&faceAttrs=%1&bodyAttrs=&cameraId=%2&startTime=%3&finishTime=%4&pageNo=%5&pageSize=%6&property=true")
            .arg(args.faceAttributList.join(','))
            .arg(args.position)
            .arg(args.start.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.end.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.page)
            .arg(args.pageCount);
    qDebug() << urlStr;
    qDebug() << postData;
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),5);
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
    resDatas->totalCount = jsObj.value("total").toInt();
    resDatas->totalPage = jsObj.value("pageNumber").toInt();
    QJsonArray jsArray = jsObj.value("data").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resDatas->data),[](const QJsonValue &jsValue){
        RestServiceI::DataRectureItem item;
        QJsonObject itemObj = jsValue.toObject();
        item.cameraId = itemObj.value("cameraId").toString();
        item.id = itemObj.value("id").toString();
        item.sceneId = itemObj.value("sceneId").toString();
        item.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        QImage img;
        img.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        item.img = img;
        item.personId = itemObj.value("personId").toString();
        return item;
    });
    return QString();
}

QString DLL::CloudHttpDao::semanticSearch(RestServiceI::SemanticSearchArgs &args, RestServiceI::SemanticReturnData *resDatas)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/find-history");
    QString postData = QObject::tr("mode=%1&faceAttrs=%2&bodyAttrs=%3&cameraId=%4&startTime=%5&finishTime=%6&pageNo=%7&pageSize=%8&property=true")
            .arg(args.mode)
            .arg(args.faceAttributList.join(','))
            .arg(args.bodyAttributeList.join(','))
            .arg(args.cameraId)
            .arg(args.startT.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.endT.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.pageNo)
            .arg(args.pageSize);
    qDebug() << urlStr;
    qDebug() << postData;
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),5);
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
    resDatas->toatal = jsObj.value("total").toInt();
    resDatas->totalPage = jsObj.value("pageNumber").toInt();
    QJsonArray jsArray = jsObj.value("data").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resDatas->records),[](const QJsonValue &jsValue){
        RestServiceI::DataRectureItem item;
        QJsonObject itemObj = jsValue.toObject();
        item.cameraId = itemObj.value("cameraId").toString();
        item.id = itemObj.value("id").toString();
        item.sceneId = itemObj.value("sceneId").toString();
        item.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        QImage img;
        img.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        item.img = img;
        item.personId = itemObj.value("personId").toString();
        return item;
    });
    return QString();
}

QString DLL::CloudHttpDao::searchByImage(RestServiceI::SearchUseImageArgs &args, QVector<RestServiceI::DataRectureItem> *resVec)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/find-history");
    QByteArray imgStr;
    QBuffer imgBuf(&imgStr);
    imgBuf.open(QIODevice::WriteOnly);
    args.image.save(&imgBuf,"jpg");
    QString base64Str(imgStr.toBase64(QByteArray::Base64UrlEncoding));
    //直接以人脸图搜索，不启用抓拍ID搜索功能
    args.faceId.clear();
    QString postData = QObject::tr("mode=%1&number=%2&similarity=%3&base64=%4&cameraId=%5&objId=%6&startTime=%7&finishTime=%8&property=false")
            .arg(args.mode)
            .arg(args.recordsCount)
            .arg(args.smilarty)
            .arg(base64Str)
            .arg(args.cameraId)
            .arg(args.faceId)
            .arg(args.startT.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.endT.toString("yyyy-MM-dd HH:mm:ss"));
    qDebug() << urlStr;
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),60);
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
    QJsonArray dataJsArray = jsObj.value("data").toArray();
    std::transform(dataJsArray.begin(),dataJsArray.end(),std::back_inserter(*resVec),[](QJsonValue jsVal){
        RestServiceI::DataRectureItem sitem;
        QJsonObject itemObj = jsVal.toObject();
        sitem.cameraId = itemObj.value("cameraId").toString();
        sitem.id = itemObj.value("id").toString();
        sitem.img.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        sitem.personId = itemObj.value("personId").toString();
        sitem.sceneId = itemObj.value("sceneId").toString();
        sitem.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        sitem.similarity = itemObj.value("similarity").toDouble();
        return sitem;
    });
    return QString();
}

QString DLL::CloudHttpDao::combinationSearch(RestServiceI::CombinationSearchArgs &args, RestServiceI::CombinationSearchReturenData *resData)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/find-history");
    QByteArray imgStr;
    QBuffer imgBuf(&imgStr);
    imgBuf.open(QIODevice::WriteOnly);
    args.img.save(&imgBuf,"jpg");
    QString base64Str(imgStr.toBase64(QByteArray::Base64UrlEncoding));
    //直接以人脸图搜索，不启用抓拍ID搜索功能
    QString postData = QObject::tr("mode=1&number=%1&faceSimilarity=%2&bodySimilarity=%3&base64=%4&cameraId=%5&startTime=%6&finishTime=%7")
            .arg(args.queryCount)
            .arg(args.similarity)
            .arg(args.tradeoff)
            .arg(base64Str)
            .arg(args.cameraId)
            .arg(args.startTime.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.endTime.toString("yyyy-MM-dd HH:mm:ss"));
    qDebug() << urlStr;
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),120);
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

    jsObj = jsObj.value("data").toObject();
    QJsonArray faceArray = jsObj.value("face").toArray();
    QJsonArray bodyArray = jsObj.value("body").toArray();
    std::transform(faceArray.begin(),faceArray.end(),std::back_inserter(resData->faceList),[](QJsonValue jsVal){
        RestServiceI::DataRectureItem sitem;
        QJsonObject itemObj = jsVal.toObject();
        sitem.cameraId = itemObj.value("cameraId").toString();
        sitem.id = itemObj.value("id").toString();
        sitem.img.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        sitem.sceneId = itemObj.value("sceneId").toString();
        sitem.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        sitem.similarity = itemObj.value("similarity").toDouble();
        return sitem;
    });
    std::transform(bodyArray.begin(),bodyArray.end(),std::back_inserter(resData->bodyList),[](QJsonValue jsVal){
        RestServiceI::CombinationScoreReturnItem sitem;
        QJsonObject itemObj = jsVal.toObject();
        sitem.cameraId = itemObj.value("cameraId").toString();
        sitem.id = itemObj.value("id").toString();
        sitem.faceImg.loadFromData(QByteArray::fromBase64(itemObj.value("faceSnapshot").toString().toLatin1()));
        sitem.bodyImg.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        sitem.sceneId = itemObj.value("sceneId").toString();
        sitem.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        sitem.similarity = itemObj.value("similarity").toDouble();
        return sitem;
    });
    return QString();
}

QString DLL::CloudHttpDao::multipleSearch(RestServiceI::MultipleSearchArgs &args, QVector<RestServiceI::MultipleSearchItem> *resVec)
{
    QString urlStr = host_ +  QObject::tr("/api/v2/external/monitor-detail/multiple-find");
    QStringList imgsBase64StrList;
    for(QImage &img : args.images){
        QByteArray imgStr;
        QBuffer imgBuf(&imgStr);
        imgBuf.open(QIODevice::WriteOnly);
        img.save(&imgBuf,"jpg");
        imgsBase64StrList << imgStr.toBase64(QByteArray::Base64UrlEncoding);
    }
#if 1
    QString postData = QObject::tr("base64s=%1&cameraId=%2&startTime=%3&finishTime=%4")
            .arg(imgsBase64StrList.join(','))
            .arg(args.cameraId)
            .arg(args.startT.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.endT.toString("yyyy-MM-dd HH:mm:ss"));
#else
    imgsBase64StrList.clear();
    imgsBase64StrList << "5c59232308c163747b3c0905" << "5c59231408c163747b3c052f";
    QString postData = QObject::tr("base64s=%1&cameraId=%2")
            .arg(imgsBase64StrList.join(','))
            .arg(args.cameraId);
#endif
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),30);
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
    QJsonArray dataJsArray = jsObj.value("data").toArray();
    std::transform(dataJsArray.begin(),dataJsArray.end(),std::back_inserter(*resVec),[](QJsonValue jsVal){
        RestServiceI::MultipleSearchItem sitem;
        QJsonObject itemObj = jsVal.toObject();
        sitem.cameraId = itemObj.value("cameraId").toString();
        sitem.img.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        sitem.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        sitem.sceneId = itemObj.value("id").toString();
        return sitem;
    });
    return QString();
}

QString DLL::CloudHttpDao::getFaceLinkDataColl(RestServiceI::FaceLinkDataCollArgs &args, RestServiceI::FaceLinkDataCollReturn *resDatas)
{
    QString urlStr = host_ +  QObject::tr("api/v2/external/monitor-detail/find-person-inlink");
    QString postData = QObject::tr("cameraId=%1&faceAttrs=%2&startTime=%3&finishTime=%4&pageNo=%5&pageSize=%6")
            .arg(args.cameraId,args.faceAttrs.join(','),args.startT.toString("yyyy-MM-dd HH:mm:ss"),args.endT.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(args.pageNo)
            .arg(args.pageSize);
    qDebug() << urlStr;
    qDebug() << postData;
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),5);
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
    resDatas->toatal = jsObj.value("total").toInt();
    resDatas->totalPage = jsObj.value("pageNumber").toInt();
    QJsonArray jsArray = jsObj.value("data").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resDatas->records),[](const QJsonValue &jsValue){
        RestServiceI::DataRectureItem item;
        QJsonObject itemObj = jsValue.toObject();
        item.cameraId = itemObj.value("cameraId").toString();
        item.id = itemObj.value("id").toString();
        item.sceneId = itemObj.value("sceneId").toString();
        item.time = QDateTime::fromMSecsSinceEpoch(itemObj.value("ts").toVariant().toULongLong());
        QImage img;
        img.loadFromData(QByteArray::fromBase64(itemObj.value("snapshot").toString().toLatin1()));
        item.img = img;
        item.personId = itemObj.value("personId").toString();
        return item;
    });
    return QString();
}

QString DLL::CloudHttpDao::eventSearch(RestServiceI::EventSearchArgs &args, RestServiceI::EventSearchReturn *resData)
{
    QString urlStr = host_ + QObject::tr("api/v2/cmcc/monitor/alarm/find?eventType=%1&sourceId=%2&pageNo=%3&pageSize=%4&startStamp=%5&endStamp=%6")
            .arg(args.alarmType,args.cameraId)
            .arg(args.pageNo)
            .arg(args.pageSize)
            .arg(args.start.toString("yyyy-MM-dd%20HH:mm:ss"),args.end.toString("yyyy-MM-dd%20HH:mm:ss"));
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),4);
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

    resData->total = jsObj.value("total").toInt();
    resData->totalPage = jsObj.value("pageNumber").toInt();
    QJsonArray jsArray = jsObj.value("data").toArray();
    std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(resData->data),[this](const QJsonValue &jsValue){
        RestServiceI::EventSearchItem item;
        QJsonObject itemObj = jsValue.toObject();
        item.id = itemObj.value("id").toString();
        item.sourceId = itemObj.value("sourceId").toString();
        item.sourceName = itemObj.value("sourceName").toString();
        item.eventType = itemObj.value("eventType").toString();
        item.sceneId = itemObj.value("sceneId").toString();
        item.bodyId = itemObj.value("bodyId").toString();
        QJsonArray jsArray = itemObj.value("warnZone").toArray();
        QVector<int> pointVec;
        std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(pointVec),[](QJsonValue jsVal){
            return jsVal.toInt();
        });
        if(!pointVec.isEmpty() && !(pointVec.count() % 2))
        {
            for(int i = 0; i < pointVec.count(); i = i + 2){
                item.warnZong << QPointF(qreal(pointVec.at(i)) / 2, qreal(pointVec.at(i + 1)) / 2);
            }
        }
        item.timeStamp = QDateTime::fromMSecsSinceEpoch(itemObj.value("timeStamp").toVariant().toULongLong());
        getImageByUrl(QObject::tr("%1api/v2/external/monitor-detail/download-image?collection=snap_scene&fieldName=snapshot&objId=%2").arg(host_,item.sceneId),&item.image);
        return item;
    });
    return QString();
}

QString DLL::CloudHttpDao::portraitLibCompSearch(RestServiceI::PortraitLibCompArgs &args, QVector<RestServiceI::PortraitLibCompItem> *resVec)
{
    QString urlStr = host_ + QObject::tr("api/v2/cmcc/portrait/search");
    QByteArray imgStr;
    QBuffer imgBuf(&imgStr);
    imgBuf.open(QIODevice::WriteOnly);
    args.image.save(&imgBuf,"jpg");
    QString base64Str(imgStr.toBase64(QByteArray::Base64UrlEncoding));
    QString postData = QObject::tr("similarity=%1&limit=%2&picture=%3&requireBase64=%4&personType=%5&token=%6")
            .arg(args.similarity)
            .arg(args.limit)
            .arg(base64Str)
            .arg(args.bRequireBase64)
            .arg(args.libType)
            //.arg(args.nPersonId)
            //.arg(args.strPersonName)
            .arg("7d1e52d3cf0142e19b5901eb1ef91372");
    if (args.nPersonId > 0)
    {
        postData.append(QString("&personId=%1").arg(args.nPersonId));
    }
    if (!args.strPersonName.isEmpty())
    {
        postData.append(QString("&personName=%1").arg(args.strPersonName));
    }
    int resCode = send(DLL::POST,urlStr.toStdString(),postData.toStdString(),60);
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
    QJsonArray dataJsArray = jsObj.value("data").toArray();
    std::transform(dataJsArray.begin(),dataJsArray.end(),std::back_inserter(*resVec),[](QJsonValue jsVal){
        RestServiceI::PortraitLibCompItem sitem;
        QJsonObject itemObj = jsVal.toObject();
        sitem.strPersonName = itemObj.value("personName").toString();
        sitem.nPersonId = itemObj.value("personId").toInt();
        sitem.faceImg.loadFromData(QByteArray::fromBase64(itemObj.value("picture").toString().toLatin1()));
        sitem.dSimilarity = itemObj.value("similarity").toDouble();
        sitem.strSubType = itemObj.value("personGroupName").toString();
        return sitem;
    });
    return QString();
}

QString DLL::CloudHttpDao::queryPersonTypes(QVector<RestServiceI::PersonType> *resVec)
{
    QString urlStr = host_ +  QObject::tr("api/v2/person/type/find?token=7d1e52d3cf0142e19b5901eb1ef91372");

    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),60);
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
    QJsonArray dataJsArray = jsObj.value("data").toArray();
    std::transform(dataJsArray.begin(),dataJsArray.end(),std::back_inserter(*resVec),[](QJsonValue jsVal){
        RestServiceI::PersonType sitem;
        QJsonObject itemObj = jsVal.toObject();
        sitem.strTypeName = itemObj.value("name").toString();
        sitem.nId = itemObj.value("id").toInt();
        sitem.strTypeNo = itemObj.value("no").toString();
        return sitem;
    });
    return QString();
}
int DLL::CloudHttpDao::progress(double totalDownLoad, double downloaded, double totalUpload, double uploaded)
{
    return 0;
}
