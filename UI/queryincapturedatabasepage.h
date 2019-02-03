#ifndef QUERYINCAPTUREDATABASEPAGE_H
#define QUERYINCAPTUREDATABASEPAGE_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QTabWidget)
QT_FORWARD_DECLARE_CLASS(SemanticSearchPage)
QT_FORWARD_DECLARE_CLASS(FaceSearch)
class QueryInCaptureDataBasePage : public WidgetI
{
    Q_OBJECT
public:
    QueryInCaptureDataBasePage(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTabWidget *tabWidget_{nullptr};
    SemanticSearchPage *semanticPage_{nullptr};
    FaceSearch *searchByImage_{nullptr};
};

#endif // QUERYINCAPTUREDATABASEPAGE_H
