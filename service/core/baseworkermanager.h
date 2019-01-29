#ifndef BASEWORKERMANAGER_H
#define BASEWORKERMANAGER_H

#include "workermanagerapi.h"
namespace BLL {
class BaseWorkerManager : public WorkerManager
{
    Q_OBJECT
public:
    explicit BaseWorkerManager(QThreadPool *thrPool, QObject*parent = nullptr) : WorkerManager(thrPool,parent){}
    ~BaseWorkerManager();
    void installWorker(QString name,Worker * w) override;
    Worker* getWorker(QString name) override;
    void startWorker(Worker*w, int threadCount = 1) override;
    void removeWorker(Worker*) override;

private:
    std::map<QString,Worker*> _workerMap;
    std::map<Worker*,int> _workerThreadCountMap;
};
}

#endif // BASEWORKERMANAGER_H
