#ifndef CPPHTTPDAO_H
#define CPPHTTPDAO_H

#include "core/httpexector.h"
#include "service/servicei.h"
namespace DLL{
class CppHttpDao : public HttpExector
{
public:
    CppHttpDao();
    QString uploadVideo(QString videoPath);
    QString uploadVideoByForm(QString videoPath);
    void setUploadProgressCallBack(std::function<void(double,double)> func);

protected:
    int progress(double totalDownLoad, double downloaded, double totalUpload, double uploaded) override;

private:
    QString host_;
    std::function<void(double,double)> uploadCallBack_;
};
}

#endif // CPPHTTPDAO_H
