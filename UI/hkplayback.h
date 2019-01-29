#ifndef HKPLAYBACK_H
#define HKPLAYBACK_H

#include <QWidget>
#include <QTimer>
#include "hkwidget.h"
#include "UI/widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(TimeAxis)

const double EPS = 1e-6;
class HKPlayBack : public QWidget
{
    Q_OBJECT
public:
    explicit HKPlayBack(QWidget *parent = nullptr);
    void startPlay(PlayBackParam &);
    void setUserStyle(WidgetManagerI::SkinStyle s);

signals:

public slots:

private:
    QPushButton *fastForward_,*rewind_{nullptr},*play_{nullptr},*stop_{nullptr};
    TimeAxis *schedule_{nullptr};
    QString playStyleStr_,pauseStyleStr_;
    PlayBackParam playBackArgs_;
    QTimer time_;
    bool isPlay_ = true;
    unsigned long long timeStuep_ = 1;
    HKWidget *videoW_{nullptr};

private slots:
    void slotPlay();
    void slotFastForward();
    void slotRewind();
    void slotStop();
    void slotUpdateTime();
    void slotValueChanged(int);
};

#endif // HKPLAYBACK_H
