#ifndef TARGETSEARCH_H
#define TARGETSEARCH_H

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
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(TreeCharts)
class TargetSearch : public WidgetI
{
    Q_OBJECT
public:
    TargetSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        QVector<item_> childrens;
    }itemData;

    //using an image control
    QWidget *imageConditionBackW_{nullptr};
    QLabel *imagePosL_{nullptr},*imageStartTL_{nullptr},*imageEndTL_{nullptr},*imageSmilarL_{nullptr},*imageCountL_{nullptr};
    QPushButton *imageBtn_{nullptr},*imageSearchBtn_{nullptr};
    QComboBox *imagePosCombox_{nullptr},*imageCountCombox_{nullptr};
    QDateTimeEdit *imageStartTimeEdit_{nullptr},*imageEndTimeEdit_{nullptr};
    QSpinBox *imageSmilarSpin_{nullptr};
    QString imageCurOid_;
    //Semantic search
    QWidget *semanticConditionBacwW_{nullptr};
    QLabel *semanticPosL_{nullptr},*semanticStartTimeL_{nullptr},*semanticEndTimeL_{nullptr};
    QComboBox *semanticPosCombox_{nullptr};
    QDateTimeEdit *semanticStartTimeEdit_{nullptr},*semanticEndTimeEdit_{nullptr};
    QPushButton *semanticSearchBtn_{nullptr};
    const int semanticPageCount_ = 40;
    QImage semanticCurImg_;
    QString semanticCurCameraId_;
    QDateTime semanticCurStartTime_,semanticCurEndTime_;

    //main page controls
    QListWidget *conditionListW_{nullptr},*dataListW_{nullptr};
    PageIndicator *pageIndicator_{nullptr};
    QWidget *leftBarBack_{nullptr},*centerBackW_{nullptr};
    QTreeWidget *treeW_{nullptr},*attributTreeW_{nullptr};
    QLabel *leftVSplitL_{nullptr};
    QLabel *centeVSplieL_{nullptr},*dataVSplitL_{nullptr};
    QStackedWidget *stackedW_{nullptr};
    QHBoxLayout *dataHlay_{nullptr};
    QImage backImg_;
    QMenu *dataMenu_{nullptr};
    bool needUpdatePageInfo_ = true;
    int curSearchType_ = -1;

    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);
    void getCameraInfo();
    void updateDataList(int totalRecords, int totalPages, QVector<std::tuple<QImage,QString,QString,QString,QDateTime>> &data);

private slots:
    void slotConditionListIndexChanged(int);
    void slotImageFaceImgBtnClicked();
    void slotImageSearchBtnClicked();
    void slotSemanticSearchBtnClicked();
    void slotOnCameraInfo(QVector<CameraInfo>);
    void slotNextPage(int);
};

#endif // TARGETSEARCH_H
