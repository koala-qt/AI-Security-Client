#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include "nodatatip.h"

NoDataTip::NoDataTip(QWidget *parent):
    QWidget(parent)
{
    imgLabel_ = new QLabel;
    errorL_ = new QLabel(QString("%1\n%2").arg(tr("No results found")).arg(tr("Please adjust the conditions to re-query")));

    QVBoxLayout *mainLay = new QVBoxLayout;
    imgLabel_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(imgLabel_);
    errorL_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(errorL_);
    setLayout(mainLay);

    if(parentWidget()){
        parentWidget()->installEventFilter(this);
    }
    setTipIcon(QPixmap("images/no-data.png"));
    setTipText(tr("<div>"
                  "<span style='color:white'>No results found</span>"
                  "<br/>"
                  "<span style='color:rgba(255,255,255,100)'>Please adjust the conditions to re-query</span>"
                  "</div>"));
}

void NoDataTip::setUserStyle(int styleIndex)
{
    if(styleIndex == 0){
    }
}

void NoDataTip::setTipIcon(QPixmap pix)
{
    imgLabel_->setPixmap(pix);
}

void NoDataTip::setTipText(QString s)
{
    errorL_->setText(s);
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
