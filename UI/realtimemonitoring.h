#ifndef REALTIMEMONITORING_H
#define REALTIMEMONITORING_H

#include <QVariant>
#include <QImage>
#include <QTreeWidgetItem>
#include "widgetinterface.h"
#include "service/notifyservicei.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(RealPlayManager)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QComboBox)
#define TIMEITEMCOUNT 4
#define FACEITEMCOUNT 8
#define EVENTITEMCOUNT 8
class RealtimeMonitoring : public WidgetI
{
    Q_OBJECT
public:
    RealtimeMonitoring(WidgetManagerI *wm,WidgetI *parent = nullptr);
    ~RealtimeMonitoring();
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QComboBox *cameraCombox_{nullptr};
    QLabel *m_timeLeftL{nullptr},*m_timeRightL{nullptr},*m_faceListL{nullptr},*m_eventListL{nullptr},*stayPersonL_{nullptr},*stayPersonTitleL_{nullptr},
    *waitingTimeTitleL_{nullptr},*waitingTimeL_{nullptr};
    QWidget * stayPersonBack_{nullptr};
    QPushButton *m_settingBtn{nullptr};
    QTreeWidget *m_treeW{nullptr};
    RealPlayManager *m_realPlayM{nullptr};
    QListWidget *m_timeList{nullptr},*m_faceList{nullptr},*m_eventList{nullptr};
    int m_timeItemH = 0;
    QString eventItemIntruderStyleSheet_,eventItemBlackAlarStyleSheet_,eventItemABDoor_,faceItemStyleSheet_;
    QSize m_faceItemSize,m_eventItemSize;
    QMenu *faceItemMenu_{nullptr},*eventItemMenu_{nullptr};
    NotifyServiceI *notifyServiceI_{nullptr};
    void updateCamera();
    void getCameraGroup(QTreeWidgetItem*, QString);
    void getCameraDevice(QTreeWidgetItem*,QString);
    QString findCameraNameById(QString &);
    int m_nBeginIndex = 0;
    int m_nEndIndex = 0;
    int totalTime_ = 0,noNumbersPersonDataCount_ = 0;
    int totalPerson_ = 0;
    QTimer *numberPersonTimer_{nullptr};
    QImage backImg_;
    QVariantMap curCameraMap_;

private slots:
    void slotCameraComboxIndexChanged(int);
    void slotTreeItemDoubleClicked(QTreeWidgetItem*,int);
    void slotAddTimeitem(QMap<QString,QVariant>);
    void slotAddFaceitem(QStringList,QImage);
    void slotAddEventitem(QStringList,QImage);
    void slotAddDevice(QVector<CameraInfo>);
    void slotOnCameraGroup(QVector<RestServiceI::CameraGoup>);
    void slotOnCameraMap(QVariantMap);
    void slotOnScenePic(QImage);
    void slotPersonStayInfoTimeout();
    void slotPersonTotalCountTimeout();
    void slotPersonCountTimer();
};

#endif // REALTIMEMONITORING_H
