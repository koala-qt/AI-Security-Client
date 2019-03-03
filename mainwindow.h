#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include "UI/widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QMenu)
//#define FULLTOP
//#define USERESIZE
class MainWindow : public WidgetI
{
    Q_OBJECT

public:
    MainWindow(WidgetI *parent = nullptr);
    ~MainWindow();

    void setUserStyle(int s = 0);

protected:
#ifdef USERESIZE
    void resizeEvent(QResizeEvent *event) override;
#endif
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QListWidget *m_topList{nullptr};
    QWidget *topWgt_{nullptr};
    QStackedWidget *m_centerW{nullptr};
    QLabel *logoLabel_{nullptr},*topBorderLine_{nullptr},*appNameL_{nullptr};
    QMenu *resourceXialaMenu_{nullptr};

private slots:
    void slotItemClicked(QListWidgetItem*);
    void slotCurentItemChanged(QListWidgetItem*,QListWidgetItem*);
};

#endif // MAINWINDOW_H
