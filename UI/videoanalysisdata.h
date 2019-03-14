#ifndef VIDEOANALYSISDATA_H
#define VIDEOANALYSISDATA_H

#include "widgetinterface.h"
#include <QMap>
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)
class PersonInfo : public WidgetI
{
    Q_OBJECT
public:
    PersonInfo(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;
    int dynamicHeight();
    void addItem(QString,QImage);
    void clearItems();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event)override;

private:
    QLabel *headImgL_{nullptr},*personOrderL_{nullptr},*personImgCountL_{nullptr};
    int itemH_ = 0,lastRowCount_ = 0,itemCount_ = 1;
    QListWidget *imgListW_{nullptr};
    QMenu *menu_{nullptr};
};

class VideoAnalysisData : public WidgetI
{
    Q_OBJECT
public:
    VideoAnalysisData(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;
    void initsize();

signals:
    void sigBackBtnClicked();

private:
    QLabel *titleTextL_{nullptr},*countTextL_{nullptr};
    QPushButton *backBtn_{nullptr};
    QListWidget *dataListW_{nullptr};

private slots:
    void slotVideoAnalysisData(QString,QImage);
};

#endif // VIDEOANALYSISDATA_H
