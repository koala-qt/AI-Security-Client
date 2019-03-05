#ifndef VIDEOANALYSISDATA_H
#define VIDEOANALYSISDATA_H

#include "widgetinterface.h"
#include <QMap>
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QMenu)
class PersonInfo : public WidgetI
{
    Q_OBJECT
public:
    PersonInfo(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;
    void addItem(QString,QImage);
    void clearItems();

private:
    QLabel *headImgL_{nullptr},*personOrderL_{nullptr},*personImgCountL_{nullptr};
    QListWidget *imgListW_{nullptr};
    QMenu *menu_{nullptr};
};

class VideoAnalysisData : public WidgetI
{
    Q_OBJECT
public:
    VideoAnalysisData(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

private:
    QLabel *titleTextL_{nullptr},*countTextL_{nullptr};
    QListWidget *dataListW_{nullptr};

    QMap<QString, PersonInfo*> personInfoMap_;

private slots:
    void slotVideoAnalysisData(QString,QImage);
};

#endif // VIDEOANALYSISDATA_H
