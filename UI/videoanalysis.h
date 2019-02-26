#ifndef VIDEOANALYSIS_H
#define VIDEOANALYSIS_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(SelectUploadVide)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(VideoAnalysisData)
QT_FORWARD_DECLARE_CLASS(UploadVideoProgress)
class VideoAnalysis : public WidgetI
{
    Q_OBJECT
public:
    VideoAnalysis(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

private:
    QStackedWidget *stackedW_{nullptr};
    VideoAnalysisData *videoDataW_{nullptr};
    UploadVideoProgress *progressW_{nullptr};
    SelectUploadVide *selectVideoW_{nullptr};

private slots:
    void slotFileSelected(QString);
};

#endif // VIDEOANALYSIS_H
