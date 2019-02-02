#ifndef COMBINATIONPAGE_H
#define COMBINATIONPAGE_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
class CombinationPage : public WidgetI
{
    Q_OBJECT
public:
    CombinationPage(WidgetManagerI *wm,WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

private:
    QWebEngineView *contentView_{nullptr};
};

#endif // COMBINATIONPAGE_H
