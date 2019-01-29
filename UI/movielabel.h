#ifndef MOVIELABEL_H
#define MOVIELABEL_H

#include <QWidget>
#include <QTimer>
class MovieLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MovieLabel(QWidget *parent = nullptr);
    void startWaring();
    void setInfo(QString);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QTimer tm_;
    QString infoStr_;
    QPoint startPoint_;
    QColor backColor_;

private slots:
    void slotTimeout();
};

#endif // MOVIELABEL_H
