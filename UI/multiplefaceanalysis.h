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
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QRadioButton)

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
    bool event(QEvent *event) override;

private slots:
    void slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data);

    void onBtnUploadFolderClicked();
    void onBtnSearchClicked();

    void onBtnOperationClicked();

private:
    void init();

    void getCameraInfo();

    void queryPersonTypes();

    void updatePersonTypesState();
    void updateBtnPersonTypeStyle(QPushButton *btn);


    void slotAddRow(QVector<RestServiceI::MNFaceAnalysisItem> info);

private:
    QGroupBox *libGroupBox_{nullptr};
    QRadioButton *markRadiuBtn_{nullptr},*registRadiuBtn_{nullptr};

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

    // 3.5 edit
    bool m_bFirstLoading = true;
    QVector<RestServiceI::PersonType> m_vecPersonTypes;
    QString m_strBigPersonType;
    QLabel *m_pLabID = Q_NULLPTR;
    QLineEdit *m_txtID = Q_NULLPTR;
    QLabel *m_pLabName = Q_NULLPTR;
    QLineEdit *m_pTxtName = Q_NULLPTR;
    QLabel *m_pLabSimilary{nullptr};
    QLineEdit *m_pTxtSimilary{nullptr};
    QWidget *m_pFaceTypesWgt{nullptr};
    QPushButton *m_pBtnOperation{nullptr};
    QList<QPushButton *> m_lstFaceLibTypes;
    QHBoxLayout *m_pFaceTypesHLay{nullptr};
    QLabel *m_pLabDbTitle{nullptr};
    QComboBox *m_pDbType{nullptr};
};

#endif // MULTIPLEFACEANALYSIS_H
