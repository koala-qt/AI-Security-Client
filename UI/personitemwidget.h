#ifndef PERSONITEMWIDGET_H
#define PERSONITEMWIDGET_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(CircleAnimation)
class PersonItemWidget : public WidgetI
{
    Q_OBJECT
public:
    PersonItemWidget(WidgetI *parent = nullptr);
    ~PersonItemWidget();
    void setInfos(const NotifyEventI::PersonEventData &);
    void setUserStyle(int s) override;

private:
    QLabel *personTypeL_{nullptr},*captureTextL_{nullptr},*registeredTextL_{nullptr},
    *captredImgL_{nullptr},*registedImgL_{nullptr},*timeIconL_{nullptr},*timeTextL_{nullptr}
    ,*posIconL_{nullptr},*posTextL_{nullptr};
    CircleAnimation *similrityL_{nullptr};
};

#endif // PERSONITEMWIDGET_H
