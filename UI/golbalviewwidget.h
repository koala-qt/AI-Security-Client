#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"

QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

private:
    void init();

private:
    QWidget *m_topWgt{nullptr};

};

#endif // GOLBALVIEWWIDGET_H
