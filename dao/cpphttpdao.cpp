#include <QSettings>
#include <QJsonDocument>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include "cpphttpdao.h"

DLL::CppHttpDao::CppHttpDao()
{
    QSettings configSetting("config.ini",QSettings::IniFormat);
    host_ = configSetting.value("Http/Cpphost").toString();
}

QString DLL::CppHttpDao::uploadVideo(QString videoPath)
{
    std::vector<std::string> headers;
    headers.emplace_back("Accept-Language: en-US,en;q=0.9");
    headers.emplace_back("Content-Type: application/binary");
    headers.emplace_back("Expect:");
    setheader(headers);
    QFile file(videoPath);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray video = file.readAll();
        std::string videoBuffer = video.toStdString();
        QString urlStr = host_ + QObject::tr("api/v1/video/analyze");
        int resCode = send(POST,urlStr.toStdString(),videoBuffer,-1);
        if(resCode != CURLE_OK){
            return curl_easy_strerror(CURLcode(resCode));
        }

        QJsonParseError jsError;
        QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
        if(jsError.error != QJsonParseError::NoError){
            return "No search results";
        }

        qDebug() << jsDoc.object();
        return QString();
    }
    return "open file failed";
}

QString DLL::CppHttpDao::uploadVideoByForm(QString videoPath)
{
    std::vector<std::string> headers;
    headers.emplace_back("Accept-Language: en-US,en;q=0.9");
    headers.emplace_back("token:7d1e52d3cf0142e19b5901eb1ef91372");
    headers.emplace_back("Expect:");
    setheader(headers);
    QFile file(videoPath);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray video = file.readAll();
        std::string videoBuffer = video.toStdString();
        QString fileName = videoPath.mid(videoPath.lastIndexOf('/') + 1);
        std::string fileNameStd = fileName.toStdString();
        QString urlStr = host_ + QObject::tr("api/v1/video/analyze");
        curl_httppost *formpost = nullptr,*lastptr = nullptr;
#if 0
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "file", //服务器接收字段
                     CURLFORM_BUFFER, fileNameStd.data(),
                     CURLFORM_BUFFERPTR, videoBuffer.data(),
                     CURLFORM_BUFFERLENGTH, videoBuffer.size(),
                     CURLFORM_CONTENTTYPE,"application/octet-stream",
                     CURLFORM_END);
#else
        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "file", //服务器接收字段
                     CURLFORM_FILE, videoPath.toLatin1().data(),
                     CURLFORM_FILENAME, fileNameStd.data(),
                     CURLFORM_END);
#endif
        int resCode = submitFormData(urlStr.toStdString(),formpost);
        if(resCode != CURLE_OK){
            return curl_easy_strerror(CURLcode(resCode));
        }

        QJsonParseError jsError;
        QJsonDocument jsDoc = QJsonDocument::fromJson(QByteArray::fromStdString(responseData()),&jsError);
        if(jsError.error != QJsonParseError::NoError){
            return "No search results";
        }

        qDebug() << jsDoc.object();
        return QString();
    }
    return "open file failed";
}

void DLL::CppHttpDao::setUploadProgressCallBack(std::function<void (double, double)> func)
{
    uploadCallBack_ = func;
}

int DLL::CppHttpDao::progress(double totalDownLoad, double downloaded, double totalUpload, double uploaded)
{
    uploadCallBack_(totalUpload,uploaded);
    return 0;
}
