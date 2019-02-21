#include <QDynamicPropertyChangeEvent>
#include <QDebug>
#include "videoanalysis.h"

VideoAnalysis::VideoAnalysis(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Video Analysis"));
}

void VideoAnalysis::setUserStyle(int s)
{

}

bool VideoAnalysis::event(QEvent *event)
{
    if(event->type() == QEvent::DynamicPropertyChange){
        QDynamicPropertyChangeEvent *ev = dynamic_cast<QDynamicPropertyChangeEvent*>(event);
        qDebug() << event << metaObject()->className();
    }

    return true;
}
