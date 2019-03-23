#ifndef CIRCLEANIMATION_H
#define CIRCLEANIMATION_H

#include <QWidget>
QT_FORWARD_DECLARE_CLASS(QPropertyAnimation)
class CircleAnimation : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
public:
    CircleAnimation(QWidget *parent = nullptr);
    void setStartValue(const qreal);
    void setEndValue(const qreal);
    void setValue(const qreal);
    qreal value() const;

signals:
    void valueChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    qreal value_ = 0;
    QPropertyAnimation *animation_{nullptr};
};

#endif // CIRCLEANIMATION_H
