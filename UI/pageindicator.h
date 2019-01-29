#ifndef PAGEINDICATOR_H
#define PAGEINDICATOR_H

#include <QWidget>
#define ITEMCOUNT 12 //大于8的偶数
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
class PageIndicator : public QWidget
{
    Q_OBJECT
public:
    PageIndicator(QWidget *parent = nullptr);
    void setPageInfo(int pageCount,int totalRecord);
    void adjustRow();
    void setUserStyle();

signals:
    void sigPageClicked(int);

private:
    int m_currPage = 0,m_changedIndex = 0,m_totalPages = 0;
    QListWidget *m_listW{nullptr};
    QLabel *m_infoL{nullptr};
    QPushButton *m_prePageBtn{nullptr},*m_nextPageBtn{nullptr};
    QSize m_itemSize;

private slots:
    void slotRowChanged(int);
    void slotPrePageClicked();
    void slotNextPageClicked();
};

#endif // PAGEINDICATOR_H
