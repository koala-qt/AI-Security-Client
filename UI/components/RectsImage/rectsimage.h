#ifndef RECTSIMAGE_H
#define RECTSIMAGE_H

#include <QWidget>
#include <QMap>
class RectsImage : public QWidget
{
    Q_OBJECT
public:
    RectsImage(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    void setRectLineColor(QColor c = Qt::yellow);
    void clearImages();
    QVector<QImage> selectedImages();
    void setInfos(const QImage backImg, const QVector<QPair<QRect,QImage>> &rectsVec);

signals:
    void sigClickedImage(QImage);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool isDown_ = false;
    qreal xPer_ = 0.0,yPer_ = 0.0;
    QImage backImg_;
    QColor rectLineColor_;
    QVector<QPair<QRect,QImage>> rectsImgVec_;
    QVector<QImage> seletedImages_;
};

#endif // RECTSIMAGE_H
