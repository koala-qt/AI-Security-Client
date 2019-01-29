#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "widgetinterface.h"
#include "service/restservicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~LoginDialog();
    void setUserStyle(WidgetManagerI::SkinStyle s);
    void autoLoginOnce(int timeDuration = 3000);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *ipL_{nullptr},*portL_{nullptr},*logoLabel_{nullptr},*logoSpitL_{nullptr},
    *logoTitleL_{nullptr},*userL_{nullptr},*pswL_{nullptr},*hostL_{nullptr};
    QWidget *logoBackW_{nullptr};
    QLineEdit *ipEdit_{nullptr},*userEdit_{nullptr},*pswEdit_{nullptr},*hostEdit_{nullptr};
    QSpinBox *portSpinBox_{nullptr};
    QDialogButtonBox *btnBox_{nullptr};
    QString hkUser_,hkPsw_;
    QPushButton *ptnSurce_{nullptr},*ptnCancell_{nullptr},*ptnMinsized_{nullptr},*ptnClose_{nullptr};

    BLL::WorkerManager *workerM_{nullptr};
    QPoint startPos_;
    bool movieable_ = false;

private slots:
    void slotSureBtnClicked();
};

#endif // LOGINDIALOG_H
