#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>
#include "windowtitlebar.h"

WindowTitleBar::WindowTitleBar(WidgetI *parent)
{
    closeBtn_ = new QPushButton(tr("×"));
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(closeBtn_);
    setLayout(mainLay);
    hide();

    setUserStyle(userStyle());
}

void WindowTitleBar::setUserStyle(int s)
{
    QFont f;
    if( s == 0){
        closeBtn_->setStyleSheet("QPushButton{"
                                 "color: rgba(255,255,255,191);"
                                 "background-color: rgba(255,255,255,0.1);"
                                 "}");
    }
}

bool WindowTitleBar::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        qDebug() << parentWidget()->isWindow() << (parentWidget()->windowFlags() & Qt::FramelessWindowHint);
        if(parentWidget() && parentWidget()->isWindow() && (parentWidget()->windowFlags() & Qt::FramelessWindowHint)){
            show();
        }
    }
    return WidgetI::event(event);
}
