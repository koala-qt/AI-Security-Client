#ifndef VIDEOANALYSIS_H
#define VIDEOANALYSIS_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(SelectUploadVide)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(VideoAnalysisData)
QT_FORWARD_DECLARE_CLASS(UploadVideoProgress)
class VideoAnalysis : public WidgetI
{
    Q_OBJECT
public:
    VideoAnalysis(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    RestServiceI *upLoadVideoService_{nullptr};
    QStackedWidget *stackedW_{nullptr};
    VideoAnalysisData *videoDataW_{nullptr};
    UploadVideoProgress *progressW_{nullptr};
    SelectUploadVide *selectVideoW_{nullptr};

private slots:
    void slotFileSelected(QString);
    void slotCancelUPload();
    void slotCurrenIndexChanged(int);
    void slotBackClicked();
};

#endif // VIDEOANALYSIS_H
