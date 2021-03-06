#ifndef SEMANTICSEARCHPAGE_H
#define SEMANTICSEARCHPAGE_H

#include "widgetinterface.h"
#include "service/servicei.h"
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
QT_FORWARD_DECLARE_CLASS(WaitingLabel)
class SemanticSearchPage : public WidgetI
{
    Q_OBJECT
public:
    SemanticSearchPage(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        QString strValue;
        int value;
        QVector<item_> childrens;
    }itemData;
    QLabel *personTypeL_{nullptr}, *posL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr};
    QComboBox *personTypeCombox_{nullptr}, *posCombox_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QPushButton *searchBtn_{nullptr},*flushAttrBtn_{nullptr};
    PageIndicator *pageIndicator_{nullptr};
    QTreeWidget *attributTreeW_{nullptr};
    QListWidget *dataListW_{nullptr};
    QLabel *centeVSplieL_{nullptr};
    QMenu *dataMenu_{nullptr};

    QString curCameraId_,curDataErrorStr_,curEnableAttrErrorStr_;
    QStringList curfaceAttrList_,curEnableAttrList_;
    QDateTime curStartTime_,curEndTime_;
    int curMode_;
    bool needUpdatePageInfo_ = true;
    bool preIsSearch_ = false;
    bool dataGeted_ = true, availabelAttrGeted_ = true;
    bool isClearnAttrSelected_ = false;
    int dataRows_;
    int dataCols_;
    int curTotalPage_,curTotalRecords_;
    QSize itemSize_,iconSize_;
    QMap<QString,QString> cameraMapInfo_;
    QVector<RestServiceI::DataRectureItem> curDataVec_;
    QVector<QPair<QString,QVector<int>>> personTypeVec_;
    NoDataTip *noDataW_{nullptr};

    void getCameraInfo();
    void setEnableAttrs(const QStringList &);
    void setTableData(QVector<RestServiceI::DataRectureItem> &data);
    QStringList checkedAttrbute(QTreeWidgetItem*);
    void getAvailableAttrs(WaitingLabel *);

private slots:
    void slotAttributeBtnClicked();
    void slotPageIndexChanged(int);
    void slotSearchAll(int,bool attrAsync);
    void slotSemanticSearch(int,bool attrAsync);
    void slotSearchFaceLink(int,bool attrAsync);
    void slotSearchBtnClicked();
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotTreeItemChanged(QTreeWidgetItem*,int);
    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);

    void onSourceCurrentIndexChanged(int index);
};

#endif // SEMANTICSEARCHPAGE_H
