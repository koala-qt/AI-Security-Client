#ifndef TRACKINGPAGE_H
#define TRACKINGPAGE_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(TrackingWebView)
QT_FORWARD_DECLARE_CLASS(QPushButton)
class TrackingPage : public WidgetI
{
    Q_OBJECT
public:
    TrackingPage( WidgetI *parent = nullptr);
    void setUserStyle(int s) override;
    void setImgageOid(QImage,QString);

private:
    QLabel *threshL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr};
    QPushButton *imgBtn_{nullptr},*searchBtn_{nullptr};
    QSpinBox *threshSpin_{nullptr};
    QDateTimeEdit *startTimeEdit_,*endTimeEdit_{nullptr};
    TrackingWebView *dataView_{nullptr};

    QString curOid_,hostname_;
    QMap<QString,RestServiceI::CameraInfo> curCameraMap_;
    void getCameraInfo();

private slots:
    void slotImgBtnClicked();
    void slotSearchBtnClicked();
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotTrackingNew(QVector<RestServiceI::TrackingReturnData>);
    void slotTracking(QVector<SearchFace>);
    void slotOnCameraClicked(QString);
    void slotOnWebError(QString);
};

#endif // TRACKINGPAGE_H
