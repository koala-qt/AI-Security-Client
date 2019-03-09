#ifndef GLVIEWMAPWIDGET_H
#define GLVIEWMAPWIDGET_H

#include "../widgetinterface.h"
#include "service/servicei.h"

#include <QPaintEvent>

QT_FORWARD_DECLARE_CLASS(QLabel)

class GlViewMapWidget : public WidgetI
{
    Q_OBJECT
public:
    GlViewMapWidget(WidgetI *parent = nullptr);

    void setUserStyle(int style) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    void init();

    void queryTopStatistics();

private:
    QImage m_backgroundImg;

    // top statistics
    QLabel *m_labLocationAccess{nullptr};
    QLabel *m_labCameraAccess{nullptr};
    QLabel *m_labIDNumbers{nullptr};
    QLabel *m_labDataStorage{nullptr};

};

#endif // GLVIEWMAPWIDGET_H
