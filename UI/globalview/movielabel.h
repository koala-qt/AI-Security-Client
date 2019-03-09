#ifndef MOVIELABEL_H
#define MOVIELABEL_H

#include <QWidget>
#include <QTimer>

#include <service/servicei.h>

QT_FORWARD_DECLARE_CLASS(QLabel)
class MovieLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MovieLabel(RestServiceI::CameraInfo info, QWidget *parent = nullptr);
    void startWaring();
    void setInfo(QString);
    int getCameraId();
    QString &getDeviceName();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;


private:
    QTimer tm_;
    QString infoStr_; // 摄像头显示信息
    QPoint startPoint_;
    QColor backColor_;
    bool m_isWarning = false;
    RestServiceI::CameraInfo m_info;

    float m_radius = 25;
    int m_fixRadius = 25;

private slots:
    void slotTimeout();
};

#endif // MOVIELABEL_H
