#ifndef COMBINATIONPAGE_H
#define COMBINATIONPAGE_H

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
QT_FORWARD_DECLARE_CLASS(QTableWidget)
class CombinationPage : public WidgetI
{
    Q_OBJECT
public:
    CombinationPage(WidgetManagerI *wm,WidgetI *parent = nullptr);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel *similarityL_{nullptr},*queryCountL_{nullptr},*positionL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr},
    *quanzhongL_{nullptr},*faceDataL_{nullptr},*bodyDataL_{nullptr};
    QWidget *faceDataBackW_{nullptr},*bodyDataBackW_{nullptr},*conditionBackW_{nullptr};
    QSpinBox *similaritySpin_{nullptr},*quanzhongSpin_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QComboBox *cameraCombox_{nullptr},*queryCountCombox_{nullptr};
    QPushButton *searchBtn_{nullptr},*imageBtn_{nullptr};
    QTableWidget *faceTable_{nullptr},*bodyTable_{nullptr};
    QMenu *faceDataMenu_{nullptr},*bodyDataMenu_{nullptr};

    QMap<QString,QString> curCameraMapInfo_;
    bool faceTableOrder_ = false,bodyTableOrder_ = false;

    void getCameraInfo();

private slots:
    void slotFaceTabelSectionClicked(int);
    void slotBodyTabelSectionClicked(int);
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotSearchBtnClicked();
    void slotImageBtnClicked();
    void slotOnSceneInfo(RestServiceI::SceneInfo sinfo);
};

#endif // COMBINATIONPAGE_H
