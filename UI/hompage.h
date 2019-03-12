#ifndef HOMPAGE_H
#define HOMPAGE_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
QT_FORWARD_DECLARE_CLASS(QWebChannel)
class HomePageWebBridge : public QObject
{
    Q_OBJECT
public:
    HomePageWebBridge(QObject *parent =nullptr);
    void setHostName(QString);

signals:
    void sigHostNameChanged(QString);
    void sigCameraClicked(QString);
    void sigWebSwitchClicked();

public slots:
    void onInitsized();
    void onCameraClicked(QString);
    void onSwitchToVideoClicked();

private:
    QString hostName_;
};

class HomPage : public WidgetI
{
    Q_OBJECT
public:
    HomPage(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;

signals:
    void sigSwitchBtnClicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event)override;

private:
    QWidget *eventBackW_{nullptr};
    QComboBox *eventCombox_{nullptr};
    QListWidget *eventListW_{nullptr};
    QWebEngineView *webView_{nullptr};
    HomePageWebBridge *webBridge_{nullptr};
    QSize eventItemSize_;
    const int eventItemCount = 5;
    NotifyServiceI *notifyServiceI_{nullptr};

private slots:
    void slotEventComboxIndexChanged(int);
    void slotOnIntruderEvent(NotifyEventI::IntruderEventData);
    void slotOnPersonEvent(NotifyEventI::PersonEventData);
    void slotOnAbDoorEvent(NotifyEventI::ABDoorEventData);
    void slotOnClimbEvent(NotifyEventI::ClimbEventData);
    void slotOngGatherEvent(NotifyEventI::GatherEventData);
    void slotOnCameraClicked(QString);
};

#endif // HOMPAGE_H
