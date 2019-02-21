#ifndef MULTIPLESEARCH_H
#define MULTIPLESEARCH_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(NoDataTip)
//#define MULTIPSEARCHUSEMOVESIZE
class MultipleSearch : public WidgetI
{
    Q_OBJECT
public:
    MultipleSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle style) override;

protected:
    bool event(QEvent *event) override;
#ifdef MULTIPSEARCHUSEMOVESIZE
    void resizeEvent(QResizeEvent *event) override;
#endif

private:
    const int itemCount_ = 4;
    QListWidget *imgList_{nullptr},*dataList_{nullptr};
    QLabel *positionL_{nullptr},
    *startTimeL_{nullptr},*endTimeL_{nullptr};
    QComboBox *posCombox_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QPushButton *searchBtn_{nullptr};
    QMenu *dataMenu_{nullptr};

    NoDataTip *noDataW_{nullptr};
    QMap<QString,QString> curCameraMapInfo_;

    void getCameraInfo();

private slots:
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotItemClicked(QListWidgetItem *);
    void slotSearchBtnClicked();
};

#endif // MULTIPLESEARCH_H
