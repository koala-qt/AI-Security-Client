#ifndef CAPTURESEARCH_H
#define CAPTURESEARCH_H

#include <QEvent>
#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(NoDataTip)
#define CAPTUREITEMROWCOUTN 6
#define CAPTUREITEMCOLCOUTN 14
class CaptureSearch : public WidgetI
{
    Q_OBJECT
public:
    CaptureSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle style) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    QDateTimeEdit *m_startTimeEdit{nullptr},*m_endTimeEdit{nullptr};
    QLabel *m_startTimeL{nullptr},*m_endTimeL{nullptr},*m_cameraL{nullptr};
    QComboBox *cameraCombox_{nullptr};
    QPushButton *m_searchBtn{nullptr};
    PageIndicator *m_pageIndicator{nullptr};
    QListWidget *m_listW{nullptr};
    QSize itemSize_,iconSize_;
    bool needUpdatePageInfo_ = false;
    QMenu *menu_{nullptr};

    NoDataTip *noDataW_{nullptr};
    QString curCameraId_;
    QDateTime curStartTime_,curEndTime_;
    QMap<QString,QString> curCameraMapInfo_;
    void getCameraInfo();

private slots:
    void slotOnSceneInfo(RestServiceI::SceneInfo sinfo);
    void slotSearchBtnClicked();
    void slotSearchSnapInfo(int page);
    void slotOnSearch(RestServiceI::CaptureSearchReturnData);
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
};

#endif // CAPTURESEARCH_H
