#ifndef SELECTUPLOADVIDE_H
#define SELECTUPLOADVIDE_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)
class SelectUploadVide : public WidgetI
{
    Q_OBJECT
public:
    SelectUploadVide(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

signals:
    void sigVideoSelected(QString);

private:
    QLabel *videoLabel_{nullptr},*videoTextL_{nullptr},*tipIconL_{nullptr},*tipTextL_{nullptr};
    QPushButton *upLoadBtn_{nullptr};

private slots:
    void slotUploadBtnClicked();
};

#endif // SELECTUPLOADVIDE_H
