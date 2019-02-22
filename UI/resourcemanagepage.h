#ifndef RESOURCEMANAGEPAGE_H
#define RESOURCEMANAGEPAGE_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
class ResourceManagePage : public WidgetI
{
    Q_OBJECT
public:
    ResourceManagePage(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;

private:
    QWebEngineView *webView_{nullptr};
};

#endif // RESOURCEMANAGEPAGE_H
