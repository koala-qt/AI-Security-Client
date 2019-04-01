#include "customwidget.h"

#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>

CustomWidget::CustomWidget(QString strGroupName, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVLay = new QVBoxLayout;
    m_btnGroup = new QPushButton(QObject::tr("Gender"));
    mainVLay->addWidget(m_btnGroup);



}
