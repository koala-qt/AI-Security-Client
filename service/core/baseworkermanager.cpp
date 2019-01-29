#include "baseworkermanager.h"
#include "workerapi.h"
#include <QThreadPool>
#include <QDebug>

BLL::BaseWorkerManager::~BaseWorkerManager()
{
    for(auto p : _workerThreadCountMap){
        for(int i = 0; i < p.second; i++){
            p.first->setWorkerManager(nullptr);
            p.first->quit();
        }
    }
    threadPool()->waitForDone();
}

void BLL::BaseWorkerManager::installWorker(QString name, BLL::Worker *w)
{
    if(w->workerType() != RunForever){
        return;
    }

    _workerMap[name] = w;
}

BLL::Worker *BLL::BaseWorkerManager::getWorker(QString name)
{
    auto iter = _workerMap.find(name);
    if(iter == _workerMap.end()){
        return nullptr;
    }
    return iter->second;
}

void BLL::BaseWorkerManager::startWorker(BLL::Worker *w, int threadCount)
{
    if(w->workerType() != RunForever){
        for(int i = 0; i < threadCount; i++){
            threadPool()->start(w);
        }
    }else{
        int havCount = threadPool()->maxThreadCount() - threadPool()->activeThreadCount();
        if(havCount < threadCount){
            qDebug() << "have no enough thread count to start alawys worker,but started" << havCount << "worker";
            threadCount = havCount;
        }
        for(int i = 0; i < threadCount; i++){
            threadPool()->start(w);
        }
        _workerThreadCountMap[w] = threadCount;
    }
}

void BLL::BaseWorkerManager::removeWorker(BLL::Worker *w)
{
    auto iter = _workerMap.begin();
    for(iter; iter != _workerMap.end(); iter++){
        Worker *exitWorker = iter->second;
        if(exitWorker == w){
            break;
        }
    }
    if(iter != _workerMap.end()){
        _workerMap.erase(iter);
        _workerThreadCountMap[w]--;
    }
}
