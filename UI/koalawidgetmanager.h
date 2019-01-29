#ifndef KOALAWIDGETMANAGER_H
#define KOALAWIDGETMANAGER_H

#include "widgetinterface.h"
class KoalaWidgetManager : public WidgetManagerI
{
public:
    explicit KoalaWidgetManager() = default;
    KoalaWidgetManager(const KoalaWidgetManager&) = delete;
    void setdefaultStyle(SkinStyle) override;
    void registerWidget(WidgetI*) override;
    void notifyUserStyle(SkinStyle) override;
    void unregisterWidget(WidgetI*) override;
    SkinStyle currentStyle() const override;

private:
    QList<WidgetI*> m_widgetList;
    SkinStyle m_curStyle = Invalid;
};

#endif // KOALAWIDGETMANAGER_H
