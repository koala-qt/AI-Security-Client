#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include "nodatatip.h"

NoDataTip::NoDataTip(QWidget *parent):
    QWidget(parent)
{
    imgLabel_ = new QLabel;
    errorL_ = new QLabel(tr("No results found"));
    tipsL_ = new QLabel(tr("Please adjust the conditions to re-query"));

    QVBoxLayout *mainLay = new QVBoxLayout;
    imgLabel_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(imgLabel_);
    errorL_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(errorL_);
    tipsL_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(tipsL_);
    setLayout(mainLay);

    imgLabel_->setPixmap(QPixmap("images/no-data.png"));
    if(parentWidget()){
        parentWidget()->installEventFilter(this);
    }
}

void NoDataTip::setUserStyle(int styleIndex)
{
    if(styleIndex == 0){
        errorL_->setStyleSheet("QLabel{"
                               "color: white;"
                               "}");
        tipsL_->setStyleSheet("QLabel{"
                               "color: rgba(255,255,255,100);"
                               "}");
    }
}

bool NoDataTip::event(QEvent *event)
{
    if(!parentWidget())return QWidget::event(event);
    if(event->type() == QEvent::Show){
        QRect r = rect();
        r.moveCenter(parentWidget()->rect().center());
        move(r.topLeft());
    }
    return QWidget::event(event);
}

bool NoDataTip::eventFilter(QObject *watched, QEvent *event)
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
