#ifndef NODATATIP_H
#define NODATATIP_H

#include <QWidget>
QT_FORWARD_DECLARE_CLASS(QLabel)
class NoDataTip : public QWidget
{
    Q_OBJECT
public:
    NoDataTip(QWidget *parent = nullptr);
    void setUserStyle(int);

protected:
    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel *imgLabel_{nullptr},*errorL_{nullptr},*tipsL_{nullptr};
};

#endif // NODATATIP_H
