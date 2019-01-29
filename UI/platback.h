#ifndef PLATBACK_H
#define PLATBACK_H

#include <QWidget>
QT_FORWARD_DECLARE_CLASS(QPropertyAnimation)
class PlatBack : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
public:
    PlatBack(QWidget *parent = nullptr);
    void loadImage(QImage &plat,QImage &shield,QImage &shadow);

signals:
    void angleChanged();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    qreal angle() const;
    void setAngle(const qreal);

    QImage plat_,shiedld_,shadow_;
    qreal angel_ = 0;
    QPropertyAnimation *animation_{nullptr};
};

#endif // PLATBACK_H
