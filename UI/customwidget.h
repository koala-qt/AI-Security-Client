#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "widgetinterface.h"
#include "dataformatdefine.h"

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QLabel)

class CustomWidget : public WidgetI
{
    Q_OBJECT
public:
    explicit CustomWidget(QString strGroupName, WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

    void addAttributes(itemData item, int startIndex = 0);

    void setEnabledAtts(const QStringList & attrs);


signals:
    void sigAppendAttr(QString strAttr);
    void sigRemoveAttr(QString strAttr);
    void sigSizeChanged();

public slots:

private slots:
    void onBtnGroupClicked();
    void onStateChanged(int state);

private:

private:
    QPushButton *m_btnGroup{nullptr};

    QWidget *m_attrContainerWgt{nullptr};
    QGridLayout *m_gridLay{nullptr};

    QList<QCheckBox *> m_lstChkBoxs;
    QList<QLabel *> m_lstLabs;
    QString m_strExpandIcon;
    QString m_strCollapseIcon;
};

#endif // CUSTOMWIDGET_H
