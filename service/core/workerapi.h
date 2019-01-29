#ifndef WORKERAPI_H
#define WORKERAPI_H

#include <QRunnable>
class QVariant;
namespace BLL {
enum WorkerType{
    RunForever,
    RunOnce
};
class WorkerManager;
class Worker : public QRunnable
{
public:
    explicit Worker(WorkerManager* wm): m_wm(wm){}
    Worker(const Worker &) = delete;
    virtual void quit() = 0;
    virtual WorkerType workerType() const = 0;
    virtual void setWorkerManager(WorkerManager* wm){m_wm = wm;}

protected:
    virtual void pushBackTask(int type,QVariant &data) = 0;
    virtual std::pair<int,QVariant> getTask(bool block = true) = 0;
    virtual WorkerManager* getWorkerManager() const{return m_wm;}

private:
    WorkerManager *m_wm = nullptr;
};
}
#endif // WORKERAPI_H
