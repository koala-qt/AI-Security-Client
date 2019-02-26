#ifndef HOMPAGE_H
#define HOMPAGE_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
class HomPage : public WidgetI
{
    Q_OBJECT
public:
    HomPage(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;

protected:
    bool event(QEvent *event) override;

private:
    QWidget *eventBackW_{nullptr};
    QComboBox *eventCombox_{nullptr};
    QListWidget *eventListW_{nullptr};
    QWebEngineView *webView_{nullptr};

    QSize eventItemSize_;
    const int eventItemCount = 5;
    NotifyServiceI *notifyServiceI_{nullptr};

private slots:
    void slotEventComboxIndexChanged(int);
    void slotOnIntruderEvent(NotifyEventI::IntruderEventData);
    void slotOnPersonEvent(NotifyEventI::PersonEventData);
    void slotOnAbDoorEvent(NotifyEventI::ABDoorEventData);
};

#endif // HOMPAGE_H
