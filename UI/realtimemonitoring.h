#ifndef REALTIMEMONITORING_H
#define REALTIMEMONITORING_H

#include <QVariant>
#include <QImage>
#include <QTreeWidgetItem>
#include <QMutex>
#include <QStack>
#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(RealPlayManager)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QStandardItemModel)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

#define FACEITEMCOUNT 8
#define EVENTITEMCOUNT 5

class RealtimeMonitoring : public WidgetI
{
    enum DbEventType
    {
        ALL = 0,
        IntruderEvent = 1,
        ABDoorEvent = 2,
        ClimbEvent = 3,
        GatherEvent = 4,
        BalckList = 5,
        VIP = 6
    };
    Q_OBJECT
public:
    RealtimeMonitoring(WidgetI *parent = nullptr);
    ~RealtimeMonitoring();
    void setUserStyle(int s) override;

signals:
    void sigSwitchBtnClicked();
    void sigEventyType(int);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event)override;

private:
    QComboBox *eventCombox_{nullptr};
    QLabel *m_faceListL{nullptr};
    QWidget *cameraGoupBackW_{nullptr},*faceCaptureBackW_{nullptr},*eventBackW_{nullptr};
    QPushButton *m_settingBtn{nullptr},*switchToMap_{nullptr};
    QTreeWidget *m_treeW{nullptr};
    RealPlayManager *m_realPlayM{nullptr};
    QListWidget *m_faceList{nullptr},*eventList_{nullptr};
    QLineEdit *posEdit_{nullptr};
    QStandardItemModel *complterModel_{nullptr};

    QImage backImg_;
    QSize eventItemSize_;
    QString faceItemStyleSheet_;
    QSize m_faceItemSize;
    QMenu *faceItemMenu_{nullptr},*eventItemMenu_{nullptr};
    NotifyServiceI *notifyServiceI_{nullptr};
    QMap<QString,QString> curCameraMap_;
    int m_curEventIndex = 0;

    void updateCamera();
    void getCameraGroup(QTreeWidgetItem*, QString);
    void getCameraDevice(QTreeWidgetItem*,QString);
    QString findCameraNameById(QString &);

private slots:
    void slotEventComboxIndexChanged(int);
    void slotTreeItemDoubleClicked(QTreeWidgetItem*,int);
    void slotAddFaceitem(NotifyPersonI::FaceSnapEventData);
    void slotOnIntruderEvent(NotifyEventI::IntruderEventData);
    void slotOnPersonEvent(NotifyEventI::PersonEventData);
    void slotOnAbDoorEvent(NotifyEventI::ABDoorEventData);
    void slotOnClimbEvent(NotifyEventI::ClimbEventData);
    void slotOngGatherEvent(NotifyEventI::GatherEventData);
    void slotAddDevice(QVector<RestServiceI::CameraInfo>);
    void slotOnCameraGroup(QVector<RestServiceI::CameraGoup>);
    void slotOnSceneInfo(RestServiceI::SceneInfo sinfo);

private:
    QStack<NotifyEventI::IntruderEventData> m_lstIntruder;
    QStack<NotifyEventI::PersonEventData> m_lstPersonVipEvent;
    QStack<NotifyEventI::PersonEventData> m_lstPersonBlackList;
    QStack<NotifyEventI::ABDoorEventData> m_lstAbDoorEvent;
    QStack<NotifyEventI::ClimbEventData> m_lstClimbeEvent;
    QStack<NotifyEventI::GatherEventData> m_lstGatherEvent;
    QMutex m_mutex;
    bool m_loadingData = false;
};

#endif // REALTIMEMONITORING_H
