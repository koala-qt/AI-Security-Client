/**
author：aihc
description：M:N
**/
#ifndef MULTIPLEFACEANALYSIS_H
#define MULTIPLEFACEANALYSIS_H

#include "widgetinterface.h"
#include "service/servicei.h"

#include <QPaintEvent>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QTableWidget)

class NoDataTip;
class PageIndicator;

class MultipleFaceAnalysis : public WidgetI
{
    enum MNColumnIndex
    {
        UploadImgCol,
        Capture1,
        Capture2,
        Capture3,
        Capture4,
        Capture5
    };
    Q_OBJECT
public:
    MultipleFaceAnalysis(WidgetI *parent = nullptr);

    void setUserStyle(int style) override;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data);

    void onBtnUploadFolderClicked();
    void onBtnSearchClicked();

private:
    void init();

    void getCameraInfo();

    void slotAddRow(QVector<RestServiceI::MNFaceAnalysisItem> info);

private:
    QPushButton *m_pBtnUploadFolder{nullptr};
    QLineEdit *m_pTxtFolderPath{nullptr};

    QLabel *m_pLabPosition{nullptr};
    QComboBox *m_pComPosition{nullptr};
    QLabel *m_pLabSTime{nullptr};
    QDateTimeEdit *m_pDateSTime{nullptr};
    QLabel *m_pLabETime{nullptr};
    QDateTimeEdit *m_pDateETime{nullptr};
    QPushButton *m_pBtnSearch{nullptr};

    PageIndicator *m_pageIndicator{nullptr};
    NoDataTip *m_pDataTip{nullptr};
    QTableWidget *m_tableW{nullptr};

    QMap<QString,QString> m_curCameraMapInfo;
    QWidget *m_pTipWgt{nullptr};

};

#endif // MULTIPLEFACEANALYSIS_H
