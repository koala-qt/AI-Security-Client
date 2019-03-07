#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"

QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int style) override;

private:
    void init();

private:
    QWidget *m_topWgt{nullptr};

    // top statistics
    QLabel *m_labLocationAccess{nullptr};
    QLabel *m_labCameraAccess{nullptr};
    QLabel *m_labIDNumbers{nullptr};
    QLabel *m_labDataStorage{nullptr};
    QPushButton *m_btnDate{nullptr};



};

#endif // GOLBALVIEWWIDGET_H
