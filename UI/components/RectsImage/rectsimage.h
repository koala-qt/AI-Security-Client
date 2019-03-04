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
    void setShowRect(bool face = true, bool body = false);
    void setRectLineColor(QColor face = Qt::yellow,QColor body = Qt::darkCyan);
    void setClickedHight(bool s = true);
    void clearImages();
    QVector<QImage> selectedImages();
    void setInfos(const QImage backImg, const QVector<QPair<QRect,QImage>> &rectsVec,const QVector<QPair<QRect,QImage>> &bodyRectImgVec);

signals:
    void sigClickedImage(QImage);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool isDown_ = false,clickedHightlight_ = false,showFaceRect_ = false,showBodyRect_ = false;
    qreal xPer_ = 0.0,yPer_ = 0.0;
    QRect hightlightRect_;
    QImage backImg_;
    QColor rectLineColor_,bodyRectLineColor_;
    QVector<QPair<QRect,QImage>> rectsImgVec_,bodyRectImgVec_;
    QVector<QImage> seletedImages_;
};

#endif // RECTSIMAGE_H
