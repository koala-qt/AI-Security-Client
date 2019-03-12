#ifndef PORTRAIT_H
#define PORTRAIT_H

#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(CornerWidget)
QT_FORWARD_DECLARE_CLASS(FlowLayout)
class Portrait : public WidgetI
{
    Q_OBJECT
public:
    Portrait( WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void slotSetData(RestServiceI::PortraitReturnData &data);

private:
    QLabel *faceImgL_{nullptr},*bodyImgL_{nullptr},
    *idL_{nullptr},*nameL_{nullptr},*personType_{nullptr};
    FlowLayout *flowLayFace_{nullptr},*flowLayBody_{nullptr};
    QPushButton *saveBtn_{nullptr};

    QPoint startP_;
    QString curObjId_;
    QImage curFaceImg_,curBodyImg_;

private slots:
    void slotSaveBtnClicked();
};

#endif // PORTRAIT_H
