#ifndef MILESTONESHAREDDATA_H
#define MILESTONESHAREDDATA_H

#include "Tools/ServerCommandServiceClient.h"
class MileStoneSharedData
{
public:
    static MileStoneSharedData *instance();
    void setClient(NmServerCommandService::ImServerCommandServiceClient *client);
    void setVmsRecorderUri(const std::string &uri);
    void setToken(const std::string &token);
    NmServerCommandService::ImServerCommandServiceClient *client() const;
    std::string token() const;
    std::string vmsRecorderUri() const;

private:
    MileStoneSharedData() = default;
    NmServerCommandService::ImServerCommandServiceClient *client_{nullptr};
    std::string vmsRecordUri_,token_;
};

#endif // MILESTONESHAREDDATA_H
