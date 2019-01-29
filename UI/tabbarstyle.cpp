#include "tabbarstyle.h"
#include <QStyleOptionTab>
#include <QDebug>

QSize TabBarStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type,option,size,widget);
    if(type == QStyle::CT_TabBarTab)
    {
        s.transpose();
    }
    return s;
}

void TabBarStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == CE_TabBarTabLabel)
    {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
        {
            QStyleOptionTab *opt = const_cast<QStyleOptionTab*>(tab);
            opt->shape = QTabBar::RoundedNorth;
//            opt->palette.setCurrentColorGroup(QPalette::Disabled);
//            opt->state |= QStyle::State_Sunken;
            QProxyStyle::drawControl(element, opt, painter, widget);
            return;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}
