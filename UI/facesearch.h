#ifndef FACESEARCH_H
#define FACESEARCH_H

#include <QImage>
#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(PageIndicator)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QMenu)

class FaceSearch : public WidgetI
{
    Q_OBJECT
public:
    FaceSearch(WidgetManagerI *wm, WidgetI *parent = nullptr);
    void setFaceImage(QImage &);
    void setOid(QString);
    void setUserStyle(WidgetManagerI::SkinStyle s) override;

public slots:
    void slotAddRow(QVector<SearchFace>);

protected:
    bool event(QEvent *event) override;

private:
    QString oidStr_;
    QMenu *menu_{nullptr};
    QTableWidget *m_tableW{nullptr};
    QPushButton *m_searchBtn{nullptr},*m_imgBtn{nullptr};
    QLabel *cameraLabel_{nullptr},*recordCountL_{nullptr},*startTimeL_{nullptr},*endTimeL_{nullptr},*similarL_{nullptr};
    QSpinBox *similarSpin_{nullptr};
    QDateTimeEdit *startTimeEdit_{nullptr},*endTimeEdit_{nullptr};
    QComboBox *topCombox_{nullptr},*cameraCombox_{nullptr};
    QImage faceImg_;
    PageIndicator *m_pageIndicator{nullptr};
    //QVector<SearchFace> m_vec;

    void getCameraInfo();

private slots:
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo>);
    void slotSearchClicked();
    void slotImgBtnClicked();
    void slotOnScenePic(QImage);
};

#endif // FACESEARCH_H
