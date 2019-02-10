#ifndef DATEANALYSIS_H
#define DATEANALYSIS_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(TrackingPage)
QT_FORWARD_DECLARE_CLASS(FacelinkTable)
QT_FORWARD_DECLARE_CLASS(QTabWidget)
QT_FORWARD_DECLARE_CLASS(FaceLinkPage)
class DateAnalysis : public WidgetI
{
    Q_OBJECT
public:
    DateAnalysis(WidgetManagerI *wm, WidgetI *parent = nullptr);
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
    TrackingPage *trackingW_{nullptr};
    FacelinkTable *faceLinkTable_{nullptr};
    FaceLinkPage *faceLinkPage_{nullptr};
    QTabWidget *tabWidgetW_{nullptr};
    QImage backImg_;

    void createTreeItem(QTreeWidget *treeW,QTreeWidgetItem *parentItem,itemData &items);

private slots:
    void slotTreeWidgetItemClicked(QTreeWidgetItem*,int);
};

#endif // DATEANALYSIS_H
