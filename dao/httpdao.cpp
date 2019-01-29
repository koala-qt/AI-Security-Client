#include "httpdao.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QUrl>
#include <QDebug>

DLL::HttpDao::HttpDao():
    HttpExector()
{
    std::vector<std::string> headers;
    headers.emplace_back("Content-Type:application/json;charset=UTF-8");
    headers.emplace_back("User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    headers.emplace_back("Expect:");
    setheader(headers);
}

int DLL::HttpDao::searchMeiaUrl(int cameraId,QString &rtspUri)
{
    QString urlStr = m_MediaHost + "api/v1/resource/mediaservers/" + QString::number(cameraId);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        qDebug() << curl_easy_strerror(CURLcode(resCode));
        return resCode;
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        qDebug() << __FILE__ << __LINE__ << jsError.errorString();
        return -1;
    }

    QJsonObject resJsObj = jsDoc.object();
    int resStatus = resJsObj.value("status").toInt();
    QString resMsg = resJsObj.value("message").toString();
    if(resStatus != 200){
        qDebug() << "response status" << resStatus << resMsg;
        return -1;
    }
    rtspUri = resJsObj.value("data").toString();
    return resCode;
}

int DLL::HttpDao::getAllDevices(QList<SecrityDevice> &devices)
{
    QString urlStr = m_deviceHost + "api/v1/resource/findchannel?pageSize=1000&pageNo=1&order=asc&sort=id";
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        qDebug() << curl_easy_strerror(CURLcode(resCode));
        return resCode;
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        qDebug() << __FILE__ << __LINE__ << jsError.errorString();
        return -1;
    }

    QJsonObject resJsObj = jsDoc.object();
    QString resMsg = resJsObj.value("message").toString();
    resCode = resJsObj.value("status").toInt();
    if(resCode != 200){
        qDebug() << "response status" << resCode << resMsg;
        return -1;
    }
    int totalPage = resJsObj.value("pageNumber").toInt();
    int totalRecords = resJsObj.value("total").toInt();

    QJsonArray cameraArray = resJsObj.value("data").toArray();
    for(int i = 0; i < cameraArray.count(); i++){
        SecrityDevice dev;
        QJsonObject camera = cameraArray.at(i).toObject();
        dev.cameraIp = camera.value("cameraIp").toString();
        dev.cameraPort = camera.value("cameraPort").toInt();
        dev.deviceName = camera.value("deviceName").toString();
        dev.id = camera.value("id").toInt();
        dev.protocol = camera.value("protocol").toString();
        dev.user = camera.value("user").toString();
        QStringList recorderList = camera.value("recordId").toString().split('-');
        if(recorderList.count() == 2){
            dev.recorderOrder = recorderList.first().toInt();
            dev.recorderChannel = recorderList.last().toInt();
        }
        devices << dev;
    }

    return CURLE_OK;
}

int DLL::HttpDao::searchWaring(HistoryWaringArgs &args, HistoryWaring &resWaring, QString &error)
{
    QString urlStr = m_searchWaringHost + "api/v1/resource/message/history/find?pageNo=";
    urlStr += QString::number(args.pageNo) + "&pageSize=";
    urlStr += QString::number(args.pageCount) + "&startTime=";
    urlStr += args.startTime.toString("yyyy-MM-dd+HH:mm:ss") + "&finishTime=";
    urlStr += args.endTime.toString("yyyy-MM-dd+HH:mm:ss") + "&type=";
    urlStr += args.waringType;
    if(args.cameraId != -1){
        urlStr += "&id=" + QString::number(args.cameraId);
    }
    qDebug() << urlStr;
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        error = QString(curl_easy_strerror(CURLcode(resCode)));
        return resCode;
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        qDebug() << jsError.errorString();
        error = jsError.errorString();
        return -1;
    }
    QJsonObject jsObj = jsDoc.object();
    resWaring.resCode = jsObj.value("status").toInt();
    resWaring.message = jsObj.value("message").toString();
    if(resWaring.resCode != 200){
        error = resWaring.message;
        return -1;
    }
    QJsonArray  dataArray = jsObj.value("data").toArray();
    for(int i = 0; i < dataArray.count(); i++){
        QJsonObject sObj = dataArray.at(i).toObject();
        resWaring.dataList << QPair<int,QDateTime>{sObj.value("id").toString().toInt(), QDateTime::fromMSecsSinceEpoch(sObj.value("time").toVariant().toULongLong())};
    }
    resWaring.totalRecors = jsObj.value("total").toInt();
    resWaring.totalPages = jsObj.value("pageNumber").toInt();
    return CURLE_OK;
}

int DLL::HttpDao::searchWaringRect(int cameraId, QVector<QPolygon> &polyons, QString &error)
{
    QString urlStr = m_searchWaringRectHost + QString("api/v1/resource/findVideoBound?channelId=%1").arg(cameraId);
    int resCode = send(DLL::GET,urlStr.toStdString(),std::string(),5);
    if(resCode != CURLE_OK){
        error = QString(curl_easy_strerror(CURLcode(resCode)));
        return resCode;
    }

    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        qDebug() << jsError.errorString();
        error = jsError.errorString();
        return -1;
    }

    QJsonObject jsObj = jsDoc.object();
    qDebug() << jsObj.value("status").toInt() << jsObj.value("message").toString();
    QJsonObject dataObj = jsObj.value("data").toObject();
    QJsonArray bounds = dataObj.value("bounds").toArray();
    for(int i = 0; i < bounds.count(); i++){
        QPolygon polyn;
        QStringList polyList = bounds.at(i).toObject().value("boundPoints").toString().split(';');
        foreach (const QString &pointStr, polyList) {
            QStringList ps = pointStr.split('/');
            if(ps.count() != 2){
                continue;
            }
            polyn << QPoint(ps.first().toInt(),ps.last().toInt());
        }
        polyons << polyn;
    }
}
