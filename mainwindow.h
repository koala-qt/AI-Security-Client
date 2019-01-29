#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include "UI/widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
//#define FULLTOP
class MainWindow : public WidgetI
{
    Q_OBJECT

public:
    MainWindow(WidgetManagerI*wm,WidgetI *parent = nullptr);
    ~MainWindow();

    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
#ifdef FULLTOP
    void resizeEvent(QResizeEvent *event) override;
#endif
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QListWidget *m_topList{nullptr};
    QWidget *topWgt_{nullptr};
    QStackedWidget *m_centerW{nullptr};
    QLabel *logoLabel_{nullptr},*topBorderLine_{nullptr};
    bool m_Drag = false;
    QPoint m_DragPosition;
};

#endif // MAINWINDOW_H
