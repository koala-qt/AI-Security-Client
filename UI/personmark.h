#ifndef PERSONMARK_H
#define PERSONMARK_H

#include <QDialog>
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
class PersonMark : public QDialog
{
    Q_OBJECT
public:
    struct PersonGroup
    {
        int id = -1;
        QString no;
        QString name;
        QString description;
    };
    PersonMark(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void setPhoto(QImage &);
    void setUserStyle(int s);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QLabel *nameL_{nullptr},*idL_{nullptr},*phontoL_{nullptr},*imgL_{nullptr},*typeL_{nullptr},*statusL_{nullptr},*customL_{nullptr},*titleL_{nullptr};
    QLineEdit *nameEdit_{nullptr},*idEdit_{nullptr},*customEdit_{nullptr};
    QComboBox *typeCombox_{nullptr},*statusCombox_{nullptr};
    QDialogButtonBox *btnBox_{nullptr};
    QPushButton *sumitBtn_{nullptr},*cancelBtn_{nullptr};

    QPoint startP_;
    QMap<QString,QVector<PersonGroup>> personTypeGroupMap_;
    void parseGroupToVec(RestServiceI::PersonGroupInfo &datas, QVector<PersonGroup> &Vec);

private slots:
    void slotOnPersonType(QVector<RestServiceI::PersonType>);
    void slotOnTypeComboxIndexChanged(int);
    void slotSubmitBtnClicked();
};

#endif // PERSONMARK_H
