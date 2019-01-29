#include "milestoneshareddata.h"

MileStoneSharedData *MileStoneSharedData::instance()
{
    static MileStoneSharedData obj;
    return &obj;
}

void MileStoneSharedData::setClient(NmServerCommandService::ImServerCommandServiceClient *client)
{
    client_ = client;
}

void MileStoneSharedData::setVmsRecorderUri(const std::string &uri)
{
    vmsRecordUri_ = uri;
}

void MileStoneSharedData::setToken(const std::string &token)
{
    token_ = token;
}

NmServerCommandService::ImServerCommandServiceClient *MileStoneSharedData::client() const
{
    return client_;
}

std::string MileStoneSharedData::token() const
{
    return token_;
}

std::string MileStoneSharedData::vmsRecorderUri() const
{
    return vmsRecordUri_;
}
