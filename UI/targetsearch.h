#ifndef TARGETSEARCH_H
#define TARGETSEARCH_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(CombinationPage)
QT_FORWARD_DECLARE_CLASS(QueryInCaptureDataBasePage)
QT_FORWARD_DECLARE_CLASS(CaptureSearch)
class TargetSearch : public WidgetI
{
    Q_OBJECT
public:
    TargetSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    typedef struct item_
    {
        QString name;
        int value;
        QVector<item_> childrens;
    }itemData;

    QTreeWidget *treeW_{nullptr};
    QStackedWidget *stackedW_{nullptr};
    QueryInCaptureDataBasePage *queryInCapture_{nullptr};
    CombinationPage *combinationSearch_{nullptr};
    CaptureSearch *capturePage_{nullptr};

    QImage backImg_;

    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);

private slots:
    void slotTreeWidgetItemClicked(QTreeWidgetItem*,int column);
};

#endif // TARGETSEARCH_H
