#ifndef MULTIPLESEARCH_H
#define MULTIPLESEARCH_H

#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
class MultipleSearch : public WidgetI
{
    Q_OBJECT
public:
    MultipleSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle style) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    const int itemCount_ = 4;
    QListWidget *imgList_{nullptr},*dataList_{nullptr};
    QLabel *similarityL_{nullptr},*queryCountL_{nullptr},*positionL_{nullptr},
    *startTimeL_{nullptr},*endTimeL_{nullptr};
    QSpinBox *similaritySpin_{nullptr};
    QComboBox *queryCountCobox_{nullptr},*posCombox_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QPushButton *searchBtn_{nullptr};
    PageIndicator *pageIndicator_{nullptr};

    QImage backImg_;
    int curTotalCount_ = 0, pageSize = 20;

    void getCameraInfo();

private slots:
    void slotPageNoChanged(int);
    void slotOnCameraInfo(QVector<CameraInfo>);
    void slotItemClicked(QListWidgetItem *);
    void slotSearchBtnClicked();
};

#endif // MULTIPLESEARCH_H
