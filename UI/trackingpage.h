#ifndef TRACKINGPAGE_H
#define TRACKINGPAGE_H

#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(TrackingWebView)
QT_FORWARD_DECLARE_CLASS(QPushButton)
class TrackingPage : public WidgetI
{
    Q_OBJECT
public:
    TrackingPage(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;
    void setImgageOid(QImage,QString);

private:
    QLabel *threshL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr};
    QPushButton *imgBtn_{nullptr},*searchBtn_{nullptr};
    QSpinBox *threshSpin_{nullptr};
    QDateTimeEdit *startTimeEdit_,*endTimeEdit_{nullptr};
    TrackingWebView *dataView_{nullptr};

    QString curOid_,hostname_;
    QMap<QString,QString> curCameraMap_;
    void getCameraInfo();

private slots:
    void slotImgBtnClicked();
    void slotSearchBtnClicked();
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotTrackingNew(QVector<RestServiceI::TrackingReturnData>);
    void slotTracking(QVector<SearchFace>);
};

#endif // TRACKINGPAGE_H
