#ifndef HOMPAGE_H
#define HOMPAGE_H

#include "widgetinterface.h"
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
    void paintEvent(QPaintEvent *event) override;

private:
    QImage backImg_;
    QComboBox *eventCombox_{nullptr};
    QListWidget *eventListW_{nullptr};
    QWebEngineView *webView_{nullptr};
};

#endif // HOMPAGE_H
