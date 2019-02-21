#ifndef WIDGETINTERFACE_H
#define WIDGETINTERFACE_H

#include <QWidget>
class WidgetI;
class WidgetManagerI
{
public:
    enum SkinStyle{
        Invalid,
        Danyahei,
        Dianyalan
    };
    virtual void setdefaultStyle(SkinStyle) = 0;
    virtual void registerWidget(WidgetI*) = 0;
    virtual void notifyUserStyle(SkinStyle) = 0;
    virtual void unregisterWidget(WidgetI*) = 0;
    virtual SkinStyle currentStyle() const = 0;
};

class WidgetI : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetI(WidgetManagerI*wm,WidgetI *parent = nullptr):_wmager(wm),QWidget(parent){
        _wmager->registerWidget(this);
    }
    ~WidgetI(){widgetManger()->unregisterWidget(this);}
    virtual void setUserStyle(WidgetManagerI::SkinStyle style) = 0;
    virtual inline WidgetManagerI::SkinStyle userStyle(){
        return _wmager->currentStyle();
    }
    virtual inline WidgetManagerI* widgetManger() const{return _wmager;}

private:
    WidgetManagerI *_wmager = nullptr;
};
#endif // WIDGETINTERFACE_H
