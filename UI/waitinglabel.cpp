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
        QRect r = rect();
        r.moveCenter(parentWidget()->rect().center());
        move(r.topLeft());
        QLabel::show();
    });
}
