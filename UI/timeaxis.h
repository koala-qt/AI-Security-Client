#ifndef TIMEAXIS_H
#define TIMEAXIS_H

#include <functional>
#include <QWidget>
#include <QPen>
#define TEXTLINESPACE 2 //文本值与刻度的垂直距离
#define SLIDERHSPACE 5  //滑块离刻度尺左边的距离
#define SLIDERLEAVESPACE 10 //刻度尺底线与当前值竖线在y方向错开的距离
#define SLIEDERNORMALWIDTH 3 //当前值刻度线，鼠标未进入时的宽度,大于1的奇数
#define SLIDERIN 4 //鼠标进入时增加的宽度
class TimeAxis : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double graduation READ graduation WRITE setGraduation NOTIFY graduationChanged)
    Q_PROPERTY(double minValue READ minValue WRITE setMinValue NOTIFY minValueChanged)
    Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(double value READ value WRITE setValue)
public:
    TimeAxis(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    void setBigTextFont(QFont &f);
    QFont bigTextFont() const;
    void setMidTextFont(QFont &f);
    QFont midTextFont() const;
    void setGraduation(double);
    void setCurTextFont(QFont &f);
    QFont curValueFont() const;
    double graduation() const;
    void setMinValue(double);
    double minValue() const;
    void setMaxValue(double);
    double maxValue() const;
    void setValue(double);
    double value() const;
    void setGerateString(std::function<QString(double&)>);
    void setPen(QPen &bottomLinePen,QPen &bigValueLinePen,QPen &midValueLinePen,QPen &graduationLinePen,QPen &bigTextPen,QPen &midTextPen,QPen &curTextPen);

signals:
    void graduationChanged();
    void minValueChanged();
    void maxValueChanged();
    void valueChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    std::function<QString(double &)> gerateString_;
    double graduation_,minValue_,maxValue_,curValue_,pixValue_ = 0.0;
    QFont bigTextFont_,midTextFont_,curValueFont_;
    QPen bottomLinePen_,bigValueLinePen_,midValueLinePen_,graduationLinePen_,bigTextPen_,midTextPen_,curTextPen_;
    QRectF sliderRect_;
    bool sliderRectInited_ = false,sliderRectIsBigState_ = false,sliderIsDown_ = false;
    qreal lastX_ = 0,hStartPos_ = 0, hEndXPos_ = 0;
};

#endif // TIMEAXIS_H
