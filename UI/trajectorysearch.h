#ifndef TRAJECTORYSEARCH_H
#define TRAJECTORYSEARCH_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(BaiduMap)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
class TrajectorySearch : public WidgetI
{
    Q_OBJECT
public:
    TrajectorySearch(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

private:
    BaiduMap *m_mapW{nullptr};
    QLabel *m_startPosL{nullptr},*m_endPosL{nullptr},*m_startTimeL{nullptr},*m_endTimeL{nullptr};
    QPushButton *m_searchBtn{nullptr};
    QComboBox *m_startPosBox{nullptr},*m_endPosBox{nullptr};
    QDateTimeEdit *m_startTimeEdit{nullptr},*m_endTimeEdit{nullptr};

private slots:
    void slotSearchBtnClicked();
};

#endif // TRAJECTORYSEARCH_H
