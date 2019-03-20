#ifndef GLVIEWMAPWIDGET_H
#define GLVIEWMAPWIDGET_H

#include "widgetinterface.h"
#include "service/servicei.h"
#include <QMutex>
#include <QPaintEvent>

QT_FORWARD_DECLARE_CLASS(QLabel)

class MovieLabel;

class GlViewMapWidget : public WidgetI
{
    Q_OBJECT
public:
    GlViewMapWidget(WidgetI *parent = nullptr);

    void setUserStyle(int style) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void slotOnIntruderEvent(NotifyEventI::IntruderEventData info);
    void slotOnAbDoorEvent(NotifyEventI::ABDoorEventData info);
    void slotOnPersonEvent(NotifyEventI::PersonEventData info);
    void slotOnClimbEvent(NotifyEventI::ClimbEventData info);
    void slotOngGatherEvent(NotifyEventI::GatherEventData info);

private:
    void init();

    void queryTopStatistics();

    void allEventWarning(NotifyEventI::GlEventData info);

private:
    QImage m_backgroundImg;

    // top statistics
    QLabel *m_labLocationAccess{nullptr}; // Area cameras
    QLabel *m_labCameraAccess{nullptr};
    QLabel *m_labIDNumbers{nullptr};
    QLabel *m_labDataStorage{nullptr};

    NotifyServiceI *m_notifyServiceI{nullptr};

    // 3.13 add cameras position control.
    QMutex m_mutex;
    QMap<QString, MovieLabel *> m_mapCameras;

};

#endif // GLVIEWMAPWIDGET_H
