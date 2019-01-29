#ifndef WORKERMANAGERAPI_H
#define WORKERMANAGERAPI_H

#include <QObject>
class QThreadPool;
class QVariant;
namespace BLL {
class Worker;
class WorkerManager : public QObject
{
    Q_OBJECT
public:
    explicit WorkerManager(QThreadPool *thrPool,QObject *parent = nullptr): _thrPool(thrPool), QObject(parent) {}
    WorkerManager(const WorkerManager&) = delete;
    virtual void installWorker(QString name, Worker * w) = 0;
    virtual Worker* getWorker(QString name) = 0;
    virtual void startWorker(Worker*w, int threadCount = 1) = 0;
    virtual void removeWorker(Worker*) = 0;
    virtual QThreadPool* threadPool() const {return _thrPool;}

private:
    QThreadPool *_thrPool = nullptr;
};
}

#endif // WORKERMANAGERAPI_H
