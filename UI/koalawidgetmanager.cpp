#include "koalawidgetmanager.h"
#include <QDebug>

void KoalaWidgetManager::setdefaultStyle(WidgetManagerI::SkinStyle s)
{
    m_curStyle = s;
}

void KoalaWidgetManager::registerWidget(WidgetI *w)
{
    m_widgetList.append(w);
}

void KoalaWidgetManager::notifyUserStyle(WidgetManagerI::SkinStyle s)
{
    if(s == m_curStyle){
        return;
    }

    for(auto w : m_widgetList){
        w->setUserStyle(s);
    }
    m_curStyle = s;
}

void KoalaWidgetManager::unregisterWidget(WidgetI *w)
{
    m_widgetList.removeOne(w);
}

WidgetManagerI::SkinStyle KoalaWidgetManager::currentStyle() const
{
    return m_curStyle;
}
