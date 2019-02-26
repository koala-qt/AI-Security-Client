#ifndef UPLOADVIDEOPROGRESS_H
#define UPLOADVIDEOPROGRESS_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QProgressBar)
QT_FORWARD_DECLARE_CLASS(QPushButton)
class UploadVideoProgress : public WidgetI
{
    Q_OBJECT
public:
    UploadVideoProgress(WidgetI *parent = nullptr);
    void setUserStyle(int s) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event)override;

public slots:
    void slotSetValue(double total, double uploaded);

private:
    QWidget *progressBackW_{nullptr},*progressBarBackW_{nullptr};
    QLabel *videoNameL_{nullptr},*progressL_{nullptr};
    QPushButton *cancelBtn_{nullptr};
    QProgressBar *progressBar_{nullptr};
};

#endif // UPLOADVIDEOPROGRESS_H
