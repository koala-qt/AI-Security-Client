#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"
#include "service/servicei.h"

QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class LeftStatisticsWebView;
class BottomStatisticsWebView;
class RightStatisticsWebView;

class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int style) override;

protected:
    bool event(QEvent *event) override;

private:
    void init();

    void queryTopStatistics();

private:
    QWidget *m_topWgt{nullptr};

    // top statistics
    QLabel *m_labLocationAccess{nullptr};
    QLabel *m_labCameraAccess{nullptr};
    QLabel *m_labIDNumbers{nullptr};
    QLabel *m_labDataStorage{nullptr};
    QPushButton *m_btnDate{nullptr};

    // web
    LeftStatisticsWebView *m_pLeftWeb{nullptr};
    BottomStatisticsWebView *m_pBottomWeb{nullptr};
    RightStatisticsWebView *m_pRightWeb{nullptr};

    // middle content
    QWidget *m_pMidWgt{nullptr};

};

#endif // GOLBALVIEWWIDGET_H
