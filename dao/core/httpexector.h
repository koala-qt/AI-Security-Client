#ifndef NETHTTPEXECTOR_H
#define NETHTTPEXECTOR_H

#include "basenetexector.h"
#include <vector>
namespace DLL {
class HttpExector : public BaseHttpExector
{
public:
    HttpExector();
    ~HttpExector();
    virtual void setheader(std::vector<std::string> &headers);
    int postFile(std::string &uri, void *formpost);
    virtual int progress(double totalDownLoad,double downloaded,double totalUpload, double uploaded) = 0;

protected:
    int send(HTTP_METHORD methord, std::string &url, std::string &data, unsigned long timeout = 1) override;
    const std::string & responseData() override;

private:
    CURL *_curl = nullptr;
    curl_slist *_headers = nullptr;
    std::string _recvData;
};
}

#endif // NETHTTPEXECTOR_H
