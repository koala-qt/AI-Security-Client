#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"
class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;
};

#endif // GOLBALVIEWWIDGET_H
