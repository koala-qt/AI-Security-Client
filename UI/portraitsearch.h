#ifndef PORTRAITSEARCH_H
#define PORTRAITSEARCH_H

#include "widgetinterface.h"
class PortraitSearch : public WidgetI
{
    Q_OBJECT
public:
    PortraitSearch(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0);
};

#endif // PORTRAITSEARCH_H
