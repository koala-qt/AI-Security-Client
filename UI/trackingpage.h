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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *threshL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr};
    QPushButton *imgBtn_{nullptr},*searchBtn_{nullptr};
    QSpinBox *threshSpin_{nullptr};
    QDateTimeEdit *startTimeEdit_,*endTimeEdit_{nullptr};
    TrackingWebView *dataView_{nullptr};

    QImage backImg_;
    QString curOid_,hostname_;

private slots:
    void slotImgBtnClicked();
    void slotSearchBtnClicked();
    void slotTracking(QVector<SearchFace>);
};

#endif // TRACKINGPAGE_H
