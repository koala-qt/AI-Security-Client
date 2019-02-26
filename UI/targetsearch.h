#ifndef TARGETSEARCH_H
#define TARGETSEARCH_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(CombinationPage)
QT_FORWARD_DECLARE_CLASS(FaceSearch)
QT_FORWARD_DECLARE_CLASS(MultipleSearch)
QT_FORWARD_DECLARE_CLASS(FaceLinkPage)
QT_FORWARD_DECLARE_CLASS(TrackingPage)
QT_FORWARD_DECLARE_CLASS(VideoAnalysis)
QT_FORWARD_DECLARE_CLASS(PortraitSearch)
QT_FORWARD_DECLARE_CLASS(SemanticSearchPage)
class TargetSearch : public WidgetI
{
    Q_OBJECT
public:
    TargetSearch( WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        int value;
        QVector<item_> childrens;
    }itemData;

    QTreeWidget *treeW_{nullptr};
    QStackedWidget *stackedW_{nullptr};
    SemanticSearchPage *semanticSearchPage_{nullptr};
    MultipleSearch *multiPleSearchPage_{nullptr};
    CombinationPage *combinationPage_{nullptr};
    FaceSearch *faceSearchPage_{nullptr};
    FaceLinkPage *facelinkPage_{nullptr};
    TrackingPage *trackingPage_{nullptr};
    PortraitSearch *portraitSearchPage_{nullptr};
    VideoAnalysis *videoAnalysisPage_{nullptr};

    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);

private slots:
    void slotTreeWidgetItemClicked(QTreeWidgetItem*,int column);
};

#endif // TARGETSEARCH_H
