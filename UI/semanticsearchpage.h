#ifndef SEMANTICSEARCHPAGE_H
#define SEMANTICSEARCHPAGE_H

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
class SemanticSearchPage : public WidgetI
{
    Q_OBJECT
public:
    SemanticSearchPage(WidgetManagerI*wm,WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        int value;
        QVector<item_> childrens;
    }itemData;
    QLabel *posL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr};
    QComboBox *posCombox_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QPushButton *searchBtn_{nullptr};
    PageIndicator *pageIndicator_{nullptr};
    QTreeWidget *attributTreeW_{nullptr};
    QListWidget *dataListW_{nullptr};
    QLabel *centeVSplieL_{nullptr};
    QMenu *dataMenu_{nullptr};

    QString curCameraId_;
    QStringList curfaceAttrList_;
    QDateTime curStartTime_,curEndTime_;
    bool needUpdatePageInfo_ = true;
    const int dataRows_ = 5;
    const int dataCols_ = 10;
    QSize itemSize_,iconSize_;
    QMap<QString,QString> cameraMapInfo_;

    void getCameraInfo();
    QStringList checkedAttrbute(QTreeWidgetItem*);

private slots:
    void slotSemanticSearch(int);
    void slotSearchBtnClicked();
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);
};

#endif // SEMANTICSEARCHPAGE_H
