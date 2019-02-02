#ifndef PORTRAIT_H
#define PORTRAIT_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(CornerWidget)
QT_FORWARD_DECLARE_CLASS(FlowLayout)
class Portrait : public WidgetI
{
public:
    Portrait(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

public slots:
    void slotSetData(QImage face,QImage body,QStringList faceAttributeList,QStringList bodyAttributeList);

private:
    CornerWidget *faceCornerW_{nullptr};
    QLabel *faceL_{nullptr},*personL_{nullptr},*splitVL_{nullptr},*personBack_{nullptr},
    *faceAttributeTitleL_,*bodyAttributeTitleL_{nullptr},*attributeSpliteL_{nullptr},
    *faceTextL{nullptr},*bodyTextL{nullptr};
    QWidget *faceAttributeBack{nullptr},*bodyAttributeBack_{nullptr};
    FlowLayout *flowLayFace_{nullptr},*flowLayBody_{nullptr};
};

#endif // PORTRAIT_H
