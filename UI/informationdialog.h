#ifndef INFORMATIONDIALOG_H
#define INFORMATIONDIALOG_H

#include <QDialog>
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
class InformationDialog : public QDialog
{
    Q_OBJECT
public:
    InformationDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    QSize sizeHint() const override;
    void setMessage(QString);
    void setUserStyle(int);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    QPoint startP_;
    QLabel *textL_{nullptr};
    QPushButton *closeBtn_{nullptr},*okBtn_{nullptr};
};

#endif // INFORMATIONDIALOG_H
