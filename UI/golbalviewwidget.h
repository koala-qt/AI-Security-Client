#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"
#include "service/servicei.h"

#include <UI/globalview/glviewmapwidget.h>

QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class LeftStatisticsWebView;
class BottomStatisticsWebView;
class RightStatisticsWebView;
class MovieLabel;

class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int style) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data);

    void slotOnIntruderEvent(NotifyEventI::IntruderEventData);

private:
    void init();

    void queryTopStatistics();

    void getCameraInfo();

    void addDevice(RestServiceI::CameraInfo &info);

    void connectEvent();

private:
    QWidget *m_topWgt{nullptr};

    QImage m_backgroundImg;

    // top statistics
    QLabel *m_labLocationAccess{nullptr};
    QLabel *m_labCameraAccess{nullptr};
    QLabel *m_labIDNumbers{nullptr};
    QLabel *m_labDataStorage{nullptr};
    QPushButton *m_btnDate{nullptr};

    // web
    LeftStatisticsWebView *m_pLeftWeb{nullptr};
    BottomStatisticsWebView *m_pBottomWeb{nullptr};
    RightStatisticsWebView *m_pRightWeb{nullptr};

    // middle content
    QWidget *m_pMidWgt{nullptr};

    //QHash<int, MovieLabel *> m_hashDevices;
    QHash<QString, MovieLabel *> m_hashDevices;
    NotifyServiceI *notifyServiceI_{nullptr};

    GlViewMapWidget *m_mapWgt{nullptr};
};

#endif // GOLBALVIEWWIDGET_H
