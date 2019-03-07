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
}

void GolbalViewWidget::setUserStyle(int s)
{

}

void GolbalViewWidget::init()
{
    QVBoxLayout *mainVLay = new QVBoxLayout;
    this->setLayout(mainVLay);
    mainVLay->setMargin(5);
    m_topWgt = new QWidget;
    mainVLay->addWidget(m_topWgt);
    QGridLayout *topGridLay = new QGridLayout;
    m_topWgt->setLayout(topGridLay);
    //QLabel *
}
