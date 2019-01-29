#ifndef SELECTIMAGE_H
#define SELECTIMAGE_H

#include <QWidget>
class SelectImage : public QWidget
{
    Q_OBJECT
public:
    SelectImage(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    void setBackImage(QImage img);
    void clearImages();
    QVector<QImage> selectedImages();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QImage originImg_,maskImg_;
    int penWidth_ = 1;
    bool _pressed = false,_closePathFlag = false,clearFlag_ = true,movingFlag_ = false;
    QPoint _endPoint;
    QRectF curRect_;
    QPolygonF curPolygon_;
    QVector<QRectF> rects_;
    int _imgWidth = 0, _imgHeight = 0;
};

#endif // SELECTIMAGE_H
