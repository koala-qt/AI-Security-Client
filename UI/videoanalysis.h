#ifndef VIDEOANALYSIS_H
#define VIDEOANALYSIS_H

#include "widgetinterface.h"
class VideoAnalysis : public WidgetI
{
    Q_OBJECT
public:
    VideoAnalysis(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    bool event(QEvent *event) override;

};

#endif // VIDEOANALYSIS_H
