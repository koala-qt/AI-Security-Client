#ifndef PORTRAITSEARCH_H
#define PORTRAITSEARCH_H

#include "widgetinterface.h"
#include "service/servicei.h"

#include <QMap>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QTabBar)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QRadioButton)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)

class PageIndicator;
class NoDataTip;
class PortraitSearch : public WidgetI
{
    Q_OBJECT
public:
    PortraitSearch(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;
    enum faceLibType
    {
        WhiteList,
        BlackList,
        TagLibrary,
        ImmigrationLibrary,
        IDCenter
    };

    enum ColumnIndex
    {
        ImageCol = 0,
        PersonNameCol,
        PersonIDCol,
        SimilarityCol,
        MainPersonType,
        SubPersonType
    };

public slots:
    void slotAddImage(QImage &img);

protected:
    bool event(QEvent *event) override;

private slots:
    void onBtnSearchClicked();
    void onBtnImgClicked();
    void onSectionClicked(int index);
    void onBtnOperationClicked();

private:
    void init();
    void slotAddRow(QVector<RestServiceI::PortraitLibCompItem> info);

    void queryPersonTypes();

    void updatePersonTypesState();
    void updateBtnPersonTypeStyle(QPushButton *btn);

private:
    QPushButton *m_btnImg = Q_NULLPTR;
    QPushButton *m_pBtnSearch = Q_NULLPTR;

    PageIndicator *m_pageIndicator = Q_NULLPTR;
    NoDataTip *m_pDataTip = Q_NULLPTR;
    QTableWidget *m_tableW = Q_NULLPTR;

    QMap<faceLibType, QString> m_mapFaceLibTypes;
    QImage m_faceImg; // upload face image.
    bool m_bFirstLoading = true;
    QVector<RestServiceI::PersonType> m_vecPersonTypes;
    QString m_strBigPersonType;

    // 3.1 add
    QLabel *m_pLabSimilary{nullptr};
    QLineEdit *m_pTxtSimilary{nullptr};

    // 3.1 edit
    QWidget *m_pFaceTypesWgt{nullptr};
    QPushButton *m_pBtnOperation{nullptr};
    QList<QPushButton *> m_lstFaceLibTypes;
    QHBoxLayout *m_pFaceTypesHLay{nullptr};

    QLabel *m_pLabDbTitle{nullptr};
    QComboBox *m_pDbType{nullptr};
};
#endif // PORTRAITSEARCH_H
