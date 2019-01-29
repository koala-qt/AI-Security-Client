#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QRegion>
#include <qdrawutil.h>
#include "buttondelegate.h"

ButtonDelegate::ButtonDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void ButtonDelegate::setText(QString s)
{
    text_ = s;
}

void ButtonDelegate::setPalette(QPalette p)
{
    pal_ = p;
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect optionRect = option.rect;
    QFont f = painter->font();
    if (optionRect.contains(mousePoint_))
    {
        if (buttonType_ == 0)
        {
            qDrawShadePanel(painter,optionRect.adjusted(5,5,-5,-5),pal_,false,1,&pal_.brush(QPalette::Button));
            //button.icon = QIcon(QString(":/Images/%1Hover").arg(m_list.at(i)));
        }
        else if (buttonType_ == 1)
        {
            qDrawShadePanel(painter,optionRect.adjusted(7,7,-7,-7),pal_,true,1,&pal_.brush(QPalette::Highlight));
            f.setPixelSize(f.pixelSize() - 4);
            //button.icon = QIcon(QString(":/Images/%1Pressed").arg(m_list.at(i)));
        }else{
            qDrawShadePanel(painter,optionRect.adjusted(7,7,-7,-7),pal_,true,1,&pal_.brush(QPalette::Highlight));
            f.setPixelSize(f.pixelSize() - 4);
        }
    }else{
        qDrawShadePanel(painter,optionRect.adjusted(5,5,-5,-5),pal_,false,1,&pal_.brush(QPalette::Button));
    }
    painter->setFont(f);
    painter->drawText(optionRect,Qt::AlignCenter,text_);
}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    buttonType_ = -1;
    QRegion r1,r2;
    QMouseEvent *mevent = static_cast<QMouseEvent*>(event);
    mousePoint_ = mevent->pos();
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        buttonType_ = 1;
        break;
    case QEvent::MouseButtonRelease:
        buttonType_ = 0;
        break;
    case QEvent::MouseMove:
        r1 = QRegion(option.rect);
        r2 = QRegion(option.rect.adjusted(5,5,-5,-5));
        if(r1.xored(r2).contains(mevent->pos())){
            buttonType_ = 0;
        }
        break;
    default:
        break;
    }
    return true;
}
