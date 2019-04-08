#include "customwidget.h"

#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>

const char *AttrValueTag = "AttrValue";

CustomWidget::CustomWidget(QString strGroupName, WidgetI *parent) :WidgetI(parent)
{
    //this->setMinimumSize(250, 170);
    m_strExpandIcon = "images/ic_expand.png";
    m_strCollapseIcon = "images/ic_unexpand.png";
    this->setFixedWidth(280);
    QVBoxLayout *mainVLay = new QVBoxLayout;
    mainVLay->setSpacing(0);
    mainVLay->setMargin(0);
    QHBoxLayout *groupHlay = new QHBoxLayout;
    m_btnGroup = new QPushButton(strGroupName);
    m_btnGroup->setIconSize(QSize(8, 5));
    m_btnGroup->setIcon(QIcon(m_strExpandIcon));
    connect(m_btnGroup, &QPushButton::clicked, this, &CustomWidget::onBtnGroupClicked);
    m_btnGroup->setFlat(true);
    groupHlay->addWidget(m_btnGroup);
    groupHlay->addStretch();
    mainVLay->addLayout(groupHlay);
    m_attrContainerWgt = new QWidget;
    m_attrContainerWgt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_gridLay = new QGridLayout;
    m_gridLay->setContentsMargins(15, 5, 5, 5);
    m_gridLay->setHorizontalSpacing(30);
    m_gridLay->setVerticalSpacing(10);
    m_attrContainerWgt->setLayout(m_gridLay);
    mainVLay->addWidget(m_attrContainerWgt);
    mainVLay->addStretch();
    this->setLayout(mainVLay);
    setUserStyle(userStyle());
}

void CustomWidget::setUserStyle(int s)
{
    if (0 == s)
    {
        //this->setStyleSheet("QWidget{background-color:transparent;color:blank;}");
        m_btnGroup->setStyleSheet("QPushButton{"
                                  "color:white;font-size:12px;"
                                  "background-color:transparent;"
                                  "text-align:left;"
                                  "}");
    }
}

void CustomWidget::addAttributes(itemData item, int startIndex)
{
    int i = 0;
    int row = 0;
    int col = 0;
    for (itemData value : item.childrens)
    {
        if ((i % 3) == 0)
        {
            row = row + 2;
            col = 0;
        }
        QCheckBox *chkBox = new QCheckBox;
        connect(chkBox, &QCheckBox::stateChanged, this, &CustomWidget::onStateChanged);
        chkBox->setProperty(AttrValueTag, value.strValue);
        chkBox->setToolTip(value.name);
        m_gridLay->addWidget(chkBox, row, col);
        m_lstChkBoxs.append(chkBox);

        chkBox->setStyleSheet(QString("QCheckBox{color:white;}"
                                      "QCheckBox::indicator{ "
                                      "padding-left:5px;width:33px;height:33px; "
                                      "image:url(images/attributes/%1.png);"
                                      "}"
                                      "QCheckBox::indicator:unchecked{ "
                                      "image:url(images/attributes/%1-1.png);"
                                      "}"
                                      "QCheckBox::indicator:checked{ "
                                      "image:url(images/attributes/%1-2.png);"
                                      "}"
                                      "QCheckBox::indicator:unchecked:disabled{"
                                      "image:url(images/attributes/%1.png); "
                                      "}"
                                      "QCheckBox::indicator:checked:disabled{"
                                      "image:url(images/attributes/%1.png); "
                                      "}").arg(QString::number(i + 1 + startIndex)));

        //qDebug() << chkBox->styleSheet();
        //QString strText =
        QLabel *layTitle = new QLabel(value.name.length() >= 7 ? value.name.left(7) + "." : value.name);
        layTitle->setStyleSheet("QLabel{color:white;font-size:10px;}QLabel:disabled{color:rgba(255,255,255,0.25);}");
        layTitle->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        //layTitle->setToolTip(value.name);
        m_gridLay->addWidget(layTitle, row + 1, col);
        m_lstLabs.append(layTitle);
        i++;
        col++;
    }

    for (auto item : m_lstChkBoxs)
    {
        item->setDisabled(true);
    }
    for (auto item : m_lstLabs)
    {
        item->setDisabled(true);
    }
}

void CustomWidget::setEnabledAtts(const QStringList &attrs)
{
    for (auto item : m_lstChkBoxs)
    {
        item->setDisabled(true);
    }
    for (auto item : m_lstLabs)
    {
        item->setDisabled(true);
    }

    for (const QString attr : attrs)
    {
        int i = 0;
        for (auto item : m_lstChkBoxs)
        {
            if (attr == item->property(AttrValueTag).toString())
            {
                item->setDisabled(false);
                m_lstLabs.at(i)->setDisabled(false);
            }
            i++;
        }
    }
}

void CustomWidget::onBtnGroupClicked()
{
    static bool isExpand = true;
    static int nWgtHight = m_attrContainerWgt->height();
    if (isExpand)
    {
        //this->setMaximumHeight(30);
        //m_attrContainerWgt->hide();
        //m_attrContainerWgt->setMaximumHeight(0);
        m_attrContainerWgt->setVisible(false);
        isExpand = false;
        m_btnGroup->setIconSize(QSize(5, 8));
        m_btnGroup->setIcon(QIcon(m_strCollapseIcon));
    }
    else
    {
        //m_attrContainerWgt->show();
        //this->setMinimumHeight(nWgtHight);
        //m_attrContainerWgt->setMaximumHeight(nWgtHight);
        m_attrContainerWgt->setVisible(true);
        m_btnGroup->setIconSize(QSize(8, 5));
        m_btnGroup->setIcon(QIcon(m_strExpandIcon));
        isExpand = true;
    }
    emit sigSizeChanged();
    //this->updateGeometry();
}

void CustomWidget::onStateChanged(int state)
{
    QCheckBox *chkWgt = static_cast<QCheckBox *>(sender());
    QString strAttr = chkWgt->property(AttrValueTag).toString();
    switch (state)
    {
    case 0:
        emit sigRemoveAttr(strAttr);
        break;
    case 2:
        emit sigAppendAttr(strAttr);
        break;
    }
}
