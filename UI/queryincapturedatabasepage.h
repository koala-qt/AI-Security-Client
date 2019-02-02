#ifndef QUERYINCAPTUREDATABASEPAGE_H
#define QUERYINCAPTUREDATABASEPAGE_H

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
class QueryInCaptureDataBasePage : public WidgetI
{
    Q_OBJECT
public:
    QueryInCaptureDataBasePage(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    bool event(QEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        int value;
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
    QStringList curfaceAttrList_,curbodyAttrList_;
    QDateTime semanticCurStartTime_,semanticCurEndTime_;

    //main page controls
    QListWidget *conditionListW_{nullptr},*dataListW_{nullptr};
    PageIndicator *pageIndicator_{nullptr};
    QTreeWidget *attributTreeW_{nullptr};
    QLabel *leftVSplitL_{nullptr};
    QLabel *centeVSplieL_{nullptr},*dataVSplitL_{nullptr};
    QStackedWidget *stackedW_{nullptr};
    QHBoxLayout *dataHlay_{nullptr};
    QMenu *dataMenu_{nullptr};
    bool needUpdatePageInfo_ = true;
    int curSearchType_ = -1;
    QMap<QString,QString> cameraMapInfo_;

    void getCameraInfo();
    void updateDataList(int totalRecords, int totalPages, QVector<std::tuple<QImage, QString, QString, QString, QDateTime, QString,QString> > &data);
    void semanticSearch(int);
    QStringList checkedAttrbute(QTreeWidgetItem*);

private slots:
    void slotConditionListIndexChanged(int);
    void slotImageFaceImgBtnClicked();
    void slotImageSearchBtnClicked();
    void slotSemanticSearchBtnClicked();
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotNextPage(int);
    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);
};

#endif // QUERYINCAPTUREDATABASEPAGE_H
