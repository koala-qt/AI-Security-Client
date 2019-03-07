#include "golbalviewwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>

GolbalViewWidget::GolbalViewWidget(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Golbal View"));
    init();
    setUserStyle(userStyle());
}

void GolbalViewWidget::setUserStyle(int style)
{
    if (0 == style)
    {
        m_topWgt->setStyleSheet("background-color:#41495C;max-height:100px;min-height:60px;color:white;");
        m_btnDate->setStyleSheet("width:100px;");
    }
}

void GolbalViewWidget::init()
{
    QVBoxLayout *mainVLay = new QVBoxLayout;
    mainVLay->setMargin(5);
    this->setLayout(mainVLay);
    m_topWgt = new QWidget;
    mainVLay->addWidget(m_topWgt);
    QGridLayout *topGridLay = new QGridLayout;
    topGridLay->setMargin(0);
    topGridLay->setColumnStretch(0, 1);
    topGridLay->setColumnStretch(1, 1);
    topGridLay->setColumnStretch(2, 1);
    topGridLay->setColumnStretch(3, 1);
    m_topWgt->setLayout(topGridLay);
    QLabel *labTitle = new QLabel(tr("Location Access"));
    labTitle->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(labTitle, 0, 0, 1, 1);
    m_labLocationAccess = new QLabel(tr("100"));
    m_labLocationAccess->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(m_labLocationAccess, 1, 0, 1, 1);
    labTitle = new QLabel(tr("Camera Access"));
    labTitle->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(labTitle, 0 , 1, 1, 1);
    m_labCameraAccess = new QLabel(tr("1200"));
    m_labCameraAccess->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(m_labCameraAccess, 1, 1, 1, 1);
    labTitle = new QLabel(tr("Total ID Numbers"));
    labTitle->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(labTitle, 0, 2, 1, 1);
    m_labIDNumbers = new QLabel(tr("710"));
    m_labIDNumbers->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(m_labIDNumbers, 1, 2, 1, 1);
    labTitle = new QLabel(tr("Data Storage"));
    labTitle->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(labTitle, 0, 3, 1, 1);
    m_labDataStorage = new QLabel(tr("10GB"));
    m_labDataStorage->setAlignment(Qt::AlignCenter);
    topGridLay->addWidget(m_labDataStorage, 1, 3, 1, 1);
    topGridLay->setAlignment(Qt::AlignCenter);
    m_btnDate = new QPushButton(tr("2019-3-15"));
    topGridLay->addWidget(m_btnDate, 0, 4, 1, 1);
    mainVLay->addStretch();
}
