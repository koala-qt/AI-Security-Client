#ifndef REALMONITORSETTING_H
#define REALMONITORSETTING_H

#include <QDialog>
#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QPushButton)
class RealMonitorSetting : public QDialog
{
    Q_OBJECT
public:
    RealMonitorSetting(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void setScreenIndex(int screenCount);
    void screenSelected(int * rows,int * cols,int *bigRow,int *bigCol,int *rowSpan,int *colSpan);
    void setUserStyle(WidgetManagerI::SkinStyle s);

private:
    QLabel *screenCountL_{nullptr},*videoL_{nullptr},*timeCostL_{nullptr},*startLocationL_{nullptr},*endLocationL_{nullptr},*arrorL_{nullptr};
    QComboBox *screenCountCombox_{nullptr},*startLocationCombox_{nullptr},*endLocationCombox_{nullptr};
    QTableWidget *timeCostTable_{nullptr};
    QPushButton *addStatisBtn_{nullptr};
    QString curStartLocation_,curEndLocation_;
    int curRmRow_;

    void addStatis();
    void updateStatis();
    void getCameraInfo();
    QString findNameById(QString);

private slots:
    void slotCellClicked(int,int);
    void slotAddStatis(bool);
    void slotRemoveStatis(bool);
    void slotSnapInfo(QVector<StatisTask>);
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotOnCameraMap(QVariantMap);
};

#endif // REALMONITORSETTING_H
