#ifndef BASEWORKER_H
#define BASEWORKER_H

#include "workerapi.h"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <QVariant>
QT_FORWARD_DECLARE_CLASS(QVariant)
namespace BLL{
class BaseWorker : public Worker
{
public:
    explicit BaseWorker(WorkerManager *wm);
    BaseWorker(const BaseWorker&) = delete;
    ~BaseWorker();
    Q_INVOKABLE void quit() override;
    virtual void pushBackTask(int type,QVariant &data) override;

protected:
    virtual std::pair<int,QVariant> getTask(bool block = true) override;

private:
    std::mutex _mtx;
    std::condition_variable _cv;
    std::queue<std::pair<int,QVariant>> _argVec;
};
}
#endif // BASEWORKER_H
