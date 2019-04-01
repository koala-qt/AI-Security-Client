#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPushButton)

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(QString strGroupName, QWidget *parent = nullptr);


signals:

public slots:

private:

private:
    QPushButton *m_btnGroup{nullptr};
};

#endif // CUSTOMWIDGET_H
