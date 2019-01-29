#include "baseworker.h"
#include <QThread>
#include <QDebug>

BLL::BaseWorker::BaseWorker(WorkerManager *wm):
    Worker(wm)
{
}

BLL::BaseWorker::~BaseWorker()
{
    qDebug() << "delete Worker";
}

void BLL::BaseWorker::quit()
{
    QVariant data;
    pushBackTask(-1,data);
}

void BLL::BaseWorker::pushBackTask(int type, QVariant &data)
{
    std::lock_guard<std::mutex> lck(_mtx);
    _argVec.push(std::make_pair(type,data));
    _cv.notify_one();
}

std::pair<int, QVariant> BLL::BaseWorker::getTask(bool block)
{
    std::pair<int,QVariant>  argData = std::make_pair(-2,QVariant());
    if(block){
        std::unique_lock<std::mutex> lck(_mtx);
        _cv.wait(lck,[this]{return !_argVec.empty();});
        argData = std::move(_argVec.front());
        _argVec.pop();
        lck.unlock();
    }else{
        std::lock_guard<std::mutex> lck(_mtx);
        if(!_argVec.empty()){
            argData = std::move(_argVec.front());
            _argVec.pop();
        }
    }
    return argData;
}
