#ifndef FACELINKTABLE_H
#define FACELINKTABLE_H

#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(NoDataTip)
class FacelinkTable : public WidgetI
{
    Q_OBJECT
public:
    FacelinkTable(WidgetManagerI*wm,WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    QLabel *posL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr};
    QComboBox *posCombox_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QPushButton *searchBtn_{nullptr};
    PageIndicator *pageIndicator_{nullptr};
    QListWidget *dataListW_{nullptr};
    QMenu *dataMenu_{nullptr};

    QString curCameraId_;
    QDateTime curStartTime_,curEndTime_;
    bool needUpdatePageInfo_ = true;
    int dataRows_;
    int dataCols_;
    QSize itemSize_,iconSize_;
    QMap<QString,QString> cameraMapInfo_;
    NoDataTip *noDataW_{nullptr};

    void getCameraInfo();

private slots:
    void slotSemanticSearch(int);
    void slotSearchBtnClicked();
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
};

#endif // FACELINKTABLE_H
