#ifndef FACELINKPAGE_H
#define FACELINKPAGE_H

#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(TreeCharts)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(WaitingLabel)
class FaceLinkPage : public WidgetI
{
    Q_OBJECT
public:
    explicit FaceLinkPage(WidgetManagerI*wm,WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;
    void setFaceLinkOidAndImg(QString, QPixmap pix);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QComboBox *levelCombox_{nullptr};
    QPushButton *imgBtn_{nullptr},*searchBtn_{nullptr},*parameterSettingBtn_{nullptr},*cancelBtn_{nullptr},*okBtn_{nullptr};
    QLabel *levelLabel_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr},*parameterL_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    TreeCharts *dataView_{nullptr};
    QWidget *parameterBackW_{nullptr};
    QListWidget *parameterSettingList_{nullptr};

    QImage backImg_;
    QString imgOid_;
    WaitingLabel *waitingL_{nullptr};

private slots:
    void slotSearchBtnClicked();
    void slotParameterBtnClicked();
    void slotFaceLinkFinished(QString);
    void slotFaceLinkTree(QJsonObject);
    void slotImgBtnClicked();
};

#endif // FACELINKPAGE_H
