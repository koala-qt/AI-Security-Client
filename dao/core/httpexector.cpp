#include "httpexector.h"
#include <functional>

DLL::HttpExector::HttpExector()
{
    _curl = curl_easy_init();
}

DLL::HttpExector::~HttpExector()
{
    curl_easy_cleanup(_curl);
}

void DLL::HttpExector::setheader(std::vector<std::string> &headers)
{
    if(_headers){
        curl_slist_free_all(_headers);
        _headers = nullptr;
    }

    for(auto &header : headers){
        _headers = curl_slist_append(_headers,header.c_str());
    }
    curl_easy_setopt(_curl,CURLOPT_HTTPHEADER,_headers);
}

size_t onReply(void *ptr,size_t size, size_t nmemb, void *userData)
{
    std::string *rvData = reinterpret_cast<std::string*>(userData);
    rvData->append(reinterpret_cast<char*>(ptr),size * nmemb);
    return size * nmemb;
}

int DLL::HttpExector::send(DLL::HTTP_METHORD methord, std::string &url, std::string &data, unsigned long timeout)
{
    curl_easy_setopt(_curl,CURLOPT_URL,url.c_str());
    switch (methord) {
    case DLL::GET:
        curl_easy_setopt(_curl,CURLOPT_POST,0);
        break;
    case DLL::POST:
        curl_easy_setopt(_curl,CURLOPT_POST,1);
        curl_easy_setopt(_curl,CURLOPT_POSTFIELDS,data.c_str());
        curl_easy_setopt(_curl,CURLOPT_POSTFIELDSIZE,data.size());
    }

    _recvData.clear();
    curl_easy_setopt(_curl,CURLOPT_WRITEDATA, &_recvData);
    curl_easy_setopt(_curl,CURLOPT_WRITEFUNCTION, onReply);
    curl_easy_setopt(_curl,CURLOPT_TIMEOUT,timeout);

    return curl_easy_perform(_curl);
}

const std::string &DLL::HttpExector::responseData()
{
    return _recvData;
}
