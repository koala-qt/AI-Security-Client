#ifndef WIDGETINTERFACE_H
#define WIDGETINTERFACE_H

#include <QWidget>
#include <QDynamicPropertyChangeEvent>
#include <QMouseEvent>
class WidgetI : public QWidget
{
    Q_OBJECT
public:
    WidgetI(QWidget *parent = nullptr):QWidget(parent){
    }
    virtual void setUserStyle(int s = 0) = 0;
    virtual int userStyle() const{return curStyle_;}

protected:
    bool event(QEvent *event) override{
        if((event->type() == QEvent::DynamicPropertyChange)){
            QDynamicPropertyChangeEvent *ev = dynamic_cast<QDynamicPropertyChangeEvent*>(event);
            curStyle_ = ev->propertyName().toInt();
            setUserStyle(curStyle_);
            ev->accept();
        }
        return QWidget::event(event);
    }

    int curStyle_ = 0;
};
#endif // WIDGETINTERFACE_H
