#ifndef EVENTSEARCH_H
#define EVENTSEARCH_H

#include <QEvent>
#include <QMap>
#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(NoDataTip)
class EventSearch : public WidgetI
{
    Q_OBJECT
public:
    EventSearch(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event)override;
    bool event(QEvent *event) override;

private:
    struct PersonGroup
    {
        int id = -1;
        QString no;
        QString name;
        QString description;
    };
    QLabel *m_startTimeL{nullptr},*m_endTimeL{nullptr},*m_positionL{nullptr},*m_waringTyleL{nullptr};
    QDateTimeEdit *m_startTimeEdit{nullptr},*m_endTimeEdit{nullptr};
    QTableWidget *m_tableW{nullptr};
    QPushButton *m_searchBtn{nullptr},*waringTypeBtn_{nullptr};
    PageIndicator *m_pageindicator{nullptr};
    QComboBox *m_positionCombox{nullptr};
    QMenu *waringTypeMenu_{nullptr};
    QWidget *centerBack_{nullptr};

    QImage backImg_;
    QString javaHost_;
    QString curCameraid_,curWaringType_,curPersonType_,curGroupNo_;
    QDateTime curStartDateTime_,curEndDateTime_;
    QMenu *menu_{nullptr};
    bool needUpdatePageInfo_ = true;
    NoDataTip *noDataTipW_{nullptr};
    QMap<QString,QColor> waringColorMap_;
    QMap<QString,QString> originalEventMap_,faceAlarmMap_;

    void getCameraInfo();
    void parseGroupToVec(RestServiceI::PersonGroupInfo &datas, QVector<PersonGroup> &Vec);

private slots:
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotSearchBtnClicked();
    void slotAlarmHistory(RestServiceI::EventSearchReturn);
    void slotSearchPageAlarmHistory(int page);
    void slotOnSceneInfo(RestServiceI::SceneInfo);
};

#endif // EVENTSEARCH_H
