#ifndef VIDEOPLAYBACK_H
#define VIDEOPLAYBACK_H

#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(HKPlayBack)
QT_FORWARD_DECLARE_CLASS(QCalendarWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
class VideoPlayback : public WidgetI
{
    Q_OBJECT
public:
    VideoPlayback(WidgetManagerI *wm, WidgetI * parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTreeWidget *deviceTree_{nullptr};
    HKPlayBack *hkplayback_{nullptr};
    QCalendarWidget *calendarWidget{nullptr};
    QTableWidget *tableWidget_{nullptr};
    QLabel *timeLabel_{nullptr},*fileLabel_{nullptr};
    QImage backImg_;

    void updateCamera();
    void getCameraGroup(QTreeWidgetItem*, QString);
    void getCameraDevice(QTreeWidgetItem*,QString);

private slots:
    void slotItemClicked(QTreeWidgetItem*,int);
    void slotAddDevice(QVector<RestServiceI::CameraInfo>);
    void slotTableItemDoubleClicked(QTableWidgetItem *);
    void slotUploadTable(QDate);
};

#endif // VIDEOPLAYBACK_H
