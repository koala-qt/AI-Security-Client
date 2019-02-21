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
    RealtimeMonitoring(WidgetManagerI *wm,WidgetI *parent = nullptr);
    ~RealtimeMonitoring();
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QComboBox *eventCombox_{nullptr};
    QLabel *m_faceListL{nullptr};
    QWidget * stayPersonBack_{nullptr};
    QPushButton *m_settingBtn{nullptr};
    QTreeWidget *m_treeW{nullptr};
    RealPlayManager *m_realPlayM{nullptr};
    QListWidget *m_faceList{nullptr},*eventList_{nullptr};

    QSize eventItemSize_;
    QString faceItemStyleSheet_;
    QSize m_faceItemSize,m_eventItemSize;
    QMenu *faceItemMenu_{nullptr},*eventItemMenu_{nullptr};
    NotifyServiceI *notifyServiceI_{nullptr};
    QImage backImg_;
    QMap<QString,QString> curCameraMap_;

    void updateCamera();
    void getCameraGroup(QTreeWidgetItem*, QString);
    void getCameraDevice(QTreeWidgetItem*,QString);
    QString findCameraNameById(QString &);

private slots:
    void slotEventComboxIndexChanged(int);
    void slotTreeItemDoubleClicked(QTreeWidgetItem*,int);
    void slotAddFaceitem(QStringList data, QImage img);
    void slotOnIntruderEvent(QStringList,QImage);
    void slotOnBlacklistEvent(QStringList,QImage);
    void slotOnAbDoorEvent(QStringList,QImage);
    void slotAddDevice(QVector<RestServiceI::CameraInfo>);
    void slotOnCameraGroup(QVector<RestServiceI::CameraGoup>);
    void slotOnSceneInfo(RestServiceI::SceneInfo sinfo);
};

#endif // REALTIMEMONITORING_H
