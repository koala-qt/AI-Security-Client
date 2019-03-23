#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include "informationdialog.h"

InformationDialog::InformationDialog(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    closeBtn_ = new QPushButton("×");
    textL_ = new QLabel;
    okBtn_ = new QPushButton(tr("Ok"));

    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(closeBtn_);
    hlay->setAlignment(Qt::AlignRight);
    mainLay->addLayout(hlay);
    textL_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(textL_);
    hlay = new QHBoxLayout;
    hlay->addWidget(okBtn_);
    hlay->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    mainLay->addLayout(hlay);
    setLayout(mainLay);

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QFont ft = closeBtn_->font();
    ft.setPixelSize(14);
    closeBtn_->setFont(ft);
    closeBtn_->setMaximumWidth(40);
    closeBtn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    textL_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textL_->setWordWrap(true);
    okBtn_->setMinimumSize(100,30);
    okBtn_->setDefault(true);
    okBtn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(closeBtn_,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(okBtn_,SIGNAL(clicked(bool)),this,SLOT(accept()));
}

QSize InformationDialog::sizeHint() const
{
    return QSize(250,160);
}

void InformationDialog::setMessage(QString str)
{
    textL_->setText(str);
}

void InformationDialog::setUserStyle(int styleIndex)
{
    QPalette pal;
    if(styleIndex == 0){
        pal = palette();
        pal.setColor(QPalette::Background,Qt::white);
        setPalette(pal);

        closeBtn_->setStyleSheet("QPushButton{"
                                 "background-color: rgb(83,77,251);"
                                 "color: white;"
                                 "border-radius: 6px;"
                                 "font-size: 18px;"
                                 "width: 24px;"
                                 "height: 24px;"
                                 "}"
                                 "QPushButton:pressed{"
                                 "padding: 2px;"
                                 "background-color: #312DA6;"
                                 "}");
        textL_->setStyleSheet("QLabel{"
                              "color: black;"
                              "}");
        okBtn_->setStyleSheet("QPushButton{"
                              "background-color: rgb(83,77,251);"
                              "color: white;"
                              "border-radius: 6px;"
                              "font-size: 18px;"
                              "}"
                              "QPushButton:pressed{"
                              "padding: 2px;"
                              "background-color: #312DA6;"
                              "}");
    }
}

void InformationDialog::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(event->button() == Qt::RightButton)return;
    startP_ = event->globalPos();
}

void InformationDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)return;
    qSwap(QPoint(),startP_);
}

void InformationDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(startP_.isNull())return;
    move(pos() + (event->globalPos() - startP_));
    startP_ = event->globalPos();
}

void InformationDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(palette().color(QPalette::Background));
    p.drawRoundRect(rect(),4,4);
}

bool InformationDialog::event(QEvent *event)
{
    if(!parentWidget())return QDialog::event(event);
    if(event->type() == QEvent::Show){
        QRect r = rect();
        r.moveCenter(parentWidget()->rect().center());
        move(parentWidget()->mapToGlobal(r.topLeft()));
    }

    return QDialog::event(event);
}
