#ifndef MOVIELABEL_H
#define MOVIELABEL_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QQueue>

#include <service/servicei.h>

QT_FORWARD_DECLARE_CLASS(QLabel)

class MovieLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal geometry READ getGeometry WRITE setGeometry NOTIFY geometryChanged)
public:
    explicit MovieLabel(NotifyEventI::IntruderEventData info, QWidget *parent = nullptr);
    void startWaring();
    void setInfo(QString);
    QString getCameraId();
    QString &getDeviceName();

    void appendWarningInfo(NotifyEventI::IntruderEventData info);

signals:
    void geometryChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void slotTimeout();

private:
    void setGeometry(const qreal geometry);
    qreal getGeometry();

private:
    QTimer tm_;
    QString infoStr_; // 摄像头显示信息
    QPoint startPoint_;
    QColor backColor_;
    QColor m_innerCircleColor;
    QColor m_outerCircleColor;
    bool m_isWarning = false;
    NotifyEventI::IntruderEventData m_info; // 入侵报警

    float m_radius = 25;
    int m_fixRadius = 25;
    qreal m_geometry = 0;
    QPropertyAnimation *animation_{nullptr};

    // 3.13 add camera position control
    QQueue<NotifyEventI::IntruderEventData> m_lstInfos;
    QMutex m_mutex;
};

#endif // MOVIELABEL_H
