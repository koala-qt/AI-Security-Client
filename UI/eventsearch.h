#ifndef EVENTSEARCH_H
#define EVENTSEARCH_H

#include <QEvent>
#include <QMap>
#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QMenu)
class EventSearch : public WidgetI
{
    Q_OBJECT
public:
    EventSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *m_startTimeL{nullptr},*m_endTimeL{nullptr},*m_positionL{nullptr},*m_waringTyleL{nullptr};
    QDateTimeEdit *m_startTimeEdit{nullptr},*m_endTimeEdit{nullptr};
    QTableWidget *m_tableW{nullptr};
    QPushButton *m_searchBtn{nullptr};
    PageIndicator *m_pageindicator{nullptr};
    QComboBox *m_positionCombox{nullptr},*m_waringTyleCombox{nullptr};
    QString curCameraid_,curWaringType_;
    QDateTime curStartDateTime_,curEndDateTime_;
    QMenu *menu_{nullptr};
    bool needUpdatePageInfo_ = true;
    QMap<QString,QString> waryingTypeMap_;
    QImage backImg_;

    void getCameraInfo();

private slots:
    void slotOnCameraInfo(QVector<CameraInfo>);
    void slotSearchBtnClicked();
    void slotAlarmHistory(PagedAlarmHis);
    void slotSearchPageAlarmHistory(int page);
    void slotOnSceneImage(QImage);
};

#endif // EVENTSEARCH_H
