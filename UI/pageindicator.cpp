﻿#include "pageindicator.h"
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>

#pragma execution_character_set("utf-8")
PageIndicator::PageIndicator(QWidget *parent):
    QWidget(parent)
{
    QHBoxLayout *mainLay = new QHBoxLayout;
    m_prePageBtn = new QPushButton;
    m_listW = new QListWidget;
    m_listW->setFlow(QListWidget::LeftToRight);
    m_listW->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_listW->setFocusPolicy(Qt::NoFocus);
    m_nextPageBtn = new QPushButton;
    m_infoL = new QLabel;
    mainLay->addWidget(m_prePageBtn);
    mainLay->addWidget(m_listW);
    mainLay->addWidget(m_nextPageBtn);
    mainLay->addWidget(m_infoL);
    mainLay->setMargin(0);
    mainLay->setAlignment(Qt::AlignCenter);
    setLayout(mainLay);

    m_listW->setFixedHeight(48);
    m_prePageBtn->setFixedSize(m_listW->height(),m_listW->height());
    m_nextPageBtn->setFixedSize(m_listW->height(),m_listW->height());
    m_prePageBtn->setFocusPolicy(Qt::NoFocus);
    m_nextPageBtn->setFocusPolicy(Qt::NoFocus);
    int itemH = m_listW->height() - m_listW->spacing() * 2 - m_listW->frameWidth() * 2;
    m_itemSize.setWidth(itemH);
    m_itemSize.setHeight(itemH);
    connect(m_listW,SIGNAL(currentRowChanged(int)),this,SLOT(slotRowChanged(int)));
    connect(m_prePageBtn,SIGNAL(clicked(bool)),this,SLOT(slotPrePageClicked()));
    connect(m_nextPageBtn,SIGNAL(clicked(bool)),this,SLOT(slotNextPageClicked()));
}

void PageIndicator::setPageInfo(int pageCount, int totalRecord)
{
    setEnabled(true);
    m_listW->clear();
    m_totalPages = pageCount;
    m_infoL->setText(QString(tr("共%1条记录 %2页")).arg(totalRecord).arg(pageCount));
    if(pageCount <= ITEMCOUNT){
        for(int i = 0; i < pageCount; i++){
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(m_itemSize);
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(QString::number(i + 1));
            m_listW->addItem(item);
        }
        m_listW->setFixedSize(QSize(pageCount * m_itemSize.width() + (pageCount + 1) * m_listW->spacing() + m_listW->frameWidth() * 2, m_listW->height()));
    }else{
        for(int i = 0; i < ITEMCOUNT; i++){
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(m_itemSize);
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(QString::number(i + 1));
            m_listW->addItem(item);
        }

        m_listW->item(ITEMCOUNT - 3)->setText("...");
        m_listW->item(ITEMCOUNT - 3)->setFlags(Qt::NoItemFlags); //ItemIsSelectable|ItemIsDragEnabled|ItemIsUserCheckable|ItemIsEnabled
        m_listW->item(ITEMCOUNT - 2)->setText(QString::number(pageCount - 1));
        m_listW->item(ITEMCOUNT - 1)->setText(QString::number(pageCount));
        m_listW->setFixedSize(QSize(ITEMCOUNT * m_itemSize.width() + (ITEMCOUNT + 1) * m_listW->spacing() + m_listW->frameWidth() * 2, m_listW->height()));
    }
    m_listW->setVisible(pageCount);
}

void PageIndicator::adjustRow()
{
    m_listW->setCurrentRow(m_listW->currentRow() + m_changedIndex);
    m_changedIndex = 0;
}

void PageIndicator::setUserStyle()
{
    m_listW->setStyleSheet("QListWidget{"
                           "border:none;"
                           "background-color: transparent;"
                           "}"
                           "QListWidget::item{"
                           "color: white;"
                           "background-color: transparent"
                           "}"
                           "QListWidget::item:hover{"
                           "background: rgba(206,206,206,40);"
                           "border: none;"
                           "}"
                           "QListWidget::item:selected{"
                           "border: 1px solid #CECECE;"
                           "border-radius: 4px;"
                           "}");

    m_prePageBtn->setStyleSheet("QPushButton{"
                               "border: 0px;"
                               "image: url(images/pre_page_btn.png);"
                               "}"
                               "QPushButton:pressed{"
                               "padding: 3px;"
                               "}");
    m_nextPageBtn->setStyleSheet("QPushButton{"
                               "border: 0px;"
                               "image: url(images/next_page_btn.png);"
                               "}"
                               "QPushButton:pressed{"
                               "padding: 3px;"
                               "}");

    QPalette pal;
    QFont f = m_infoL->font();
    f.setPixelSize(15);
    m_infoL->setFont(f);
    pal = m_infoL->palette();
    pal.setColor(QPalette::Foreground,Qt::white);
    m_infoL->setPalette(pal);
}
#include <QDebug>
void PageIndicator::slotRowChanged(int row)
{
    if(!m_listW->currentItem() || m_listW->currentItem()->text() == "..." || m_currPage == m_listW->currentItem()->text().toInt()){
        return;
    }

    m_currPage = m_listW->currentItem()->text().toInt();
    m_changedIndex = 0;
    if(m_totalPages > ITEMCOUNT){
        int offset = (ITEMCOUNT - 2 - 3 - 1) / 2;
        int middle = 1 + offset + 1;
        if(m_currPage + 3 >= m_totalPages){
            m_listW->item(1)->setText("...");
            m_listW->item(1)->setFlags(Qt::NoItemFlags);
            for(int i = 0; i < ITEMCOUNT - 2; i++){
                m_listW->item(2 + i)->setText(QString::number(m_totalPages - (ITEMCOUNT - 2 - 1) + i));
            }
            m_listW->item(ITEMCOUNT - 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            m_changedIndex = m_currPage - m_listW->currentItem()->text().toInt();
        }else if(m_currPage - middle > 0 && m_currPage + 3 < m_totalPages){
            m_listW->item(1)->setText("...");
            m_listW->item(1)->setFlags(Qt::NoItemFlags);
            m_listW->item(ITEMCOUNT - 3)->setText("...");
            m_listW->item(ITEMCOUNT - 3)->setFlags(Qt::NoItemFlags);
            if(row > 1 && row < middle){
                for(int i = 0; i < ITEMCOUNT - 5; i++){
                    int temp = m_listW->item(i + 2)->text().toInt();
                    m_listW->item(i + 2)->setText(QString::number(temp + row - middle));
                }
                m_changedIndex = middle - row;
            }else if(row > middle && row < ITEMCOUNT - 3){
                for(int i = 0; i < ITEMCOUNT - 5; i++){
                    int temp = m_listW->item(i + 2)->text().toInt();
                    m_listW->item(i + 2)->setText(QString::number(temp + row - middle));
                }
                m_changedIndex = middle - row;
            }
        }else if(m_currPage - middle <= 0){
            m_listW->item(ITEMCOUNT - 3)->setText("...");
            m_listW->item(ITEMCOUNT - 3)->setFlags(Qt::NoItemFlags);
            m_listW->item(1)->setText("2");
            m_listW->item(1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            for(int i = 0; i < ITEMCOUNT - 3; i++){
                m_listW->item(i)->setText(QString::number(i + 1));
            }
            m_changedIndex = m_currPage - m_listW->currentItem()->text().toInt();
        }
    }

    emit sigPageClicked(m_currPage);
//    QTimer::singleShot(30,this,[this]{m_listW->setCurrentRow(m_listW->currentRow() + m_changedIndex);});
}

void PageIndicator::slotPrePageClicked()
{
    if(m_listW->currentRow() == -1 || m_listW->currentRow() == 0 || m_listW->count() == 0){
        return;
    }

    if(m_listW->item(m_listW->currentRow() - 1)->text() == "..."){
        m_listW->setCurrentRow(m_listW->currentRow() - 2);
    }else{
        m_listW->setCurrentRow(m_listW->currentRow() - 1);
    }
}

void PageIndicator::slotNextPageClicked()
{
    if(m_listW->currentRow() == m_listW->count() - 1 || m_listW->count() == 0){
        return;
    }

    if(m_listW->item(m_listW->currentRow() + 1)->text() == "..."){
        if(m_totalPages >= m_listW->currentRow() + 2){
            m_listW->setCurrentRow(m_listW->currentRow() + 2);
        }
    }else{
        if(m_totalPages >= m_listW->currentRow() + 1){
            m_listW->setCurrentRow(m_listW->currentRow() + 1);
        }
    }
}
