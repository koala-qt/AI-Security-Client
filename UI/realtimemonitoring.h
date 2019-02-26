#ifndef REALTIMEMONITORING_H
#define REALTIMEMONITORING_H

#include <QVariant>
#include <QImage>
#include <QTreeWidgetItem>
#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(RealPlayManager)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QComboBox)
#define FACEITEMCOUNT 8
#define EVENTITEMCOUNT 5
class RealtimeMonitoring : public WidgetI
{
    Q_OBJECT
public:
    RealtimeMonitoring(WidgetI *parent = nullptr);
    ~RealtimeMonitoring();
    void setUserStyle(int s) override;

protected:
    bool event(QEvent *event) override;

private:
    QComboBox *eventCombox_{nullptr};
    QLabel *m_faceListL{nullptr};
    QWidget *cameraGoupBackW_{nullptr},*faceCaptureBackW_{nullptr},*eventBackW_{nullptr};
    QPushButton *m_settingBtn{nullptr};
    QTreeWidget *m_treeW{nullptr};
    RealPlayManager *m_realPlayM{nullptr};
    QListWidget *m_faceList{nullptr},*eventList_{nullptr};
    QLineEdit *posEdit_{nullptr};
    QStringList cameraStringList_;

    QSize eventItemSize_;
    QString faceItemStyleSheet_;
    QSize m_faceItemSize,m_eventItemSize;
    QMenu *faceItemMenu_{nullptr},*eventItemMenu_{nullptr};
    NotifyServiceI *notifyServiceI_{nullptr};
    QMap<QString,QString> curCameraMap_;

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
    void slotAddDevice(QVector<RestServiceI::CameraInfo>);
    void slotOnCameraGroup(QVector<RestServiceI::CameraGoup>);
    void slotOnSceneInfo(RestServiceI::SceneInfo sinfo);
};

#endif // REALTIMEMONITORING_H
