#ifndef CAPTURESEARCH_H
#define CAPTURESEARCH_H

#include "widgetinterface.h"
#include "service/restservicei.h"

#include <QEvent>
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
#define CAPTUREITEMROWCOUTN 6
#define CAPTUREITEMCOLCOUTN 10
class CaptureSearch : public WidgetI
{
    Q_OBJECT
public:
    CaptureSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle style) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        QVector<item_> childrens;
    }itemData;

    QDateTimeEdit *m_startTimeEdit{nullptr},*m_endTimeEdit{nullptr};
    QLabel *m_startTimeL{nullptr},*m_endTimeL{nullptr},*m_cameraL{nullptr};
    QComboBox *cameraCombox_{nullptr};
    QPushButton *m_searchBtn{nullptr};
    PageIndicator *m_pageIndicator{nullptr};
    QListWidget *m_listW{nullptr};
    QTreeWidget *attributeTreeW_{nullptr};
    QSize itemSize_,iconSize_;
    bool needUpdatePageInfo_ = false;
    QMenu *menu_{nullptr};

    QString curCameraId_;
    QDateTime curStartTime_,curEndTime_;
    void getCameraInfo();
    void createTreeItem(QTreeWidgetItem *parentItem,itemData &items);

private slots:
    void slotOnScenePic(QImage);
    void slotSearchBtnClicked();
    void slotSearchSnapInfo(int page);
    void slotOnSearch(PagedSnapFaceHis);
    void slotOnCameraInfo(QVector<CameraInfo>);
};

#endif // CAPTURESEARCH_H
