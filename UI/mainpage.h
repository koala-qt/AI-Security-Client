#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QChartGlobal>
QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QBarSet;
class QBarSeries;
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(RectNumberWidget)
QT_FORWARD_DECLARE_CLASS(RadarChart)
QT_FORWARD_DECLARE_CLASS(QCalendarWidget)
QT_FORWARD_DECLARE_CLASS(PlatBack)
QT_FORWARD_DECLARE_CLASS(MovieLabel)
QT_FORWARD_DECLARE_CLASS(CalendarEcharts)
#include "widgetinterface.h"
#include "service/notifyservicei.h"
#include "service/restservicei.h"
#define MAINPAGEEVENTITEMCOUNT 7
//#define USEQCALENDAR

class MainPage : public WidgetI
{
    Q_OBJECT
public:
    MainPage(WidgetManagerI* wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void resizeEvent(QResizeEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void slotAddImgItem(QStringList,QImage);
    void slotEventComboxClicked(int);
    void slotAreaGraphics(QVector<QPointF>);
    void slotAlarmData(int,int,int);
    void slotFaceGrab(int,int,int,int);
    void slotTotalEventStatics(int,int);
    void slotSpiderData(QVector<QVector<double>>);
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotPieChart(QVector<kf::PieCharData>);

private:
    QChartView *pieChartView_{nullptr},*areaChartView_{nullptr},*barChartView_{nullptr};
    QListWidget *eventList_{nullptr};
    QComboBox *eventCobox_{nullptr};
    QLabel *eventStaticsticsTitleLabel_{nullptr},*eventCalenderLabel_{nullptr},*grabPersonStaticsLabel_{nullptr},*grabePersonSumLabel_{nullptr},*grabPersonSumValueLabel_{nullptr},
    *registerPersonLabel_{nullptr},*registerPersonValueLabel_{nullptr},*accessPersonLabel_{nullptr},*accessPersonValueLabel_{nullptr},*controlPersonLabel_{nullptr}
    ,*controlPersonValueLabel_{nullptr},*abDoorAlarmLabel_{nullptr},*intrdeAlarmLabel_{nullptr},*blackAlarmLabel{nullptr},*abDoorAlarmValueL_{nullptr},*intrudeAlarmValueL_{nullptr},*blackAlarmValueL_{nullptr},
    *centerTitleL_{nullptr};
    QWidget *eventStatisticsBackW_{nullptr},*grabPersonStatisticsBack_{nullptr},*eventCalenderBack_{nullptr},*eventListBack_{nullptr},
    *gaojingBackW_{nullptr},*handedBackW_{nullptr},*nonHandedBackW_{nullptr};
    RadarChart *eventDistribution_{nullptr};
    RectNumberWidget *eventStaticsLabel_{nullptr},*importEventLabel_{nullptr};
#ifdef  USEQCALENDAR
    QCalendarWidget *eventCalender_{nullptr};
#else
    CalendarEcharts *eventCalender_{nullptr};
#endif
    PlatBack *mapCenterBack_{nullptr};
    QImage backImage_;

    QMap<QString,QColor> pieChartColor_,spiderColor_;

    NotifyServiceI *notifyService_{nullptr};
    QMap<QString,MovieLabel*> movieLabelMap_;
    QSize imgItemSize_;
    void getCameraInfo();
    int m_nListWgtHeight = 880;
    int m_nListWgtWidth = 306;
};

#endif // MAINPAGE_H
