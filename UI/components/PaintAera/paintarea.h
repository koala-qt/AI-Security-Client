#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = nullptr);
    void setCanvasBackground(QPixmap);
    QVector<QPolygonF> polygons() const;
    QSize sizeHint() const override;
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool _startFlag = false, _pressed = false,_closePathFlag = false;
    QPixmap backPix_;
    QPoint _endPoint;
    QPolygonF curPolygon_;
    QVector<QPolygonF> polygons_;
    int _imgWidth = 0, _imgHeight = 0;
};

#endif // PAINTAREA_H
