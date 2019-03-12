#ifndef WINDOWTITLEBAR_H
#define WINDOWTITLEBAR_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QPushButton)
class WindowTitleBar : public WidgetI
{
    Q_OBJECT
public:
    WindowTitleBar(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    bool event(QEvent *event) override;

private:
    QPushButton *closeBtn_{nullptr};
};

#endif // WINDOWTITLEBAR_H
