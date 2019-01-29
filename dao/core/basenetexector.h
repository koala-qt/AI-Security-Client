#ifndef BASENETEXECTOR_H
#define BASENETEXECTOR_H

#include <string>
namespace DLL {
enum NetType{
    TCP,
    UDP
};

enum HTTP_METHORD{
    POST,
    GET
};

class BaseHttpExector
{
public:
    BaseHttpExector() = default;
    virtual ~BaseHttpExector(){}

protected:
    virtual int send(HTTP_METHORD methord,std::string &uri,std::string &data, unsigned long timeout = 1) = 0;
    virtual const std::string & responseData() = 0;
};

class BaseSocketExector{
public:
    BaseSocketExector() = default;

protected:
    virtual int createSocket() = 0;
    virtual int connect(std::string& ip, int port) = 0;
    virtual int sendData(std::string &data) = 0;
    virtual size_t recvData(void *buffer, unsigned long size) = 0;
    virtual void close() = 0;
};
}
#endif // BASENETEXECTOR_H
