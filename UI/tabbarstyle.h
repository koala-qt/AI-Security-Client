#ifndef TABBARSTYLE_H
#define TABBARSTYLE_H

#include <QProxyStyle>
class TabBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;
    void drawControl(ControlElement element,const QStyleOption *option,QPainter *painter,const QWidget *widget) const override;
};

#endif // TABBARSTYLE_H
