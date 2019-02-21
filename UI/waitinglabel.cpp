#include <QMovie>
#include <QDebug>
#include <QTimer>
#include "waitinglabel.h"

WaitingLabel::WaitingLabel(QWidget *parent):
    QLabel(parent)
{
    movie_ = new QMovie("images/waiting.gif");
    setMovie(movie_);
}

void WaitingLabel::setMovieFile(QString f)
{
    movie_->setFileName(f);
}

void WaitingLabel::show(int timeout)
{
    QTimer::singleShot(timeout,this,[&]{
        movie_->start();
        resize(movie_->frameRect().size());
        QLabel::show();
    });
}

bool WaitingLabel::event(QEvent *event)
{
    if(!parentWidget())return QWidget::event(event);
    if(event->type() == QEvent::Show){
        QRect r = rect();
        r.moveCenter(parentWidget()->rect().center());
        move(r.topLeft());
    }
    return QWidget::event(event);
}

bool WaitingLabel::eventFilter(QObject *watched, QEvent *event)
{
    if(!parentWidget()){
        return QWidget::eventFilter(watched,event);
    }

    QWidget *pw = qobject_cast<QWidget*>(watched);
    if(pw == parentWidget() && event->type() == QEvent::Resize){
        QRect r = rect();
        r.moveCenter(parentWidget()->rect().center());
        move(r.topLeft());
    }

    return QWidget::eventFilter(watched,event);
}
