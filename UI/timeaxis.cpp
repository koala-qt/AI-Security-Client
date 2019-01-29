#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QCursor>
#include <QDebug>
#include <qdrawutil.h>
#include "timeaxis.h"

TimeAxis::TimeAxis(QWidget *parent):
    QWidget(parent)
{
    setMouseTracking(true);
}

QSize TimeAxis::sizeHint() const
{
    return QSize(100,60);
}

void TimeAxis::setBigTextFont(QFont &f)
{
    bigTextFont_ = f;
}

QFont TimeAxis::bigTextFont() const
{
    return bigTextFont_;
}

void TimeAxis::setMidTextFont(QFont &f)
{
    midTextFont_ = f;
}

QFont TimeAxis::midTextFont() const
{
    return midTextFont_;
}

void TimeAxis::setGraduation(double v)
{
    if(graduation_ != v){
        emit graduationChanged();
    }
    graduation_ = v;
}

void TimeAxis::setCurTextFont(QFont &f)
{
    curValueFont_ = f;
}

QFont TimeAxis::curValueFont() const
{
    return curValueFont_;
}

double TimeAxis::graduation() const
{
    return graduation_;
}

void TimeAxis::setMinValue(double v)
{
    if(minValue_ != v){
        emit minValueChanged();
    }
    minValue_ = v;
}

double TimeAxis::minValue() const
{
    return minValue_;
}

void TimeAxis::setMaxValue(double v)
{
    if(maxValue_ != v){
        emit maxValue_;
    }
    maxValue_ = v;
}

double TimeAxis::maxValue() const
{
    return maxValue_;
}

void TimeAxis::setValue(double v)
{
    curValue_ = v;
    update();
}

double TimeAxis::value() const
{
    return curValue_;
}

void TimeAxis::setGerateString(std::function<QString (double &)> f)
{
    gerateString_ = f;
}

void TimeAxis::setPen(QPen &bottomLinePen, QPen &bigValueLinePen, QPen &midValueLinePen, QPen &graduationLinePen, QPen &bigTextPen, QPen &midTextPen, QPen &curTextPen)
{
    bottomLinePen_ = bottomLinePen;
    bigValueLinePen_ = bigValueLinePen;
    midValueLinePen_ = midValueLinePen;
    graduationLinePen_ = graduationLinePen;
    bigTextPen_ = bigTextPen;
    midTextPen_ = midTextPen;
    curTextPen_ = curTextPen;
}

void TimeAxis::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int lineCount = static_cast<int>(::ceil(qAbs(maxValue_ - minValue_)/graduation())) + 1;
    QFontMetrics bigFontMetrics(bigTextFont());
    int bigTextFontH = bigFontMetrics.height();
    QFontMetrics midFontMetrics(midTextFont());
    QFontMetrics curFontMetrics(curValueFont());
    int hSpace = SLIDERHSPACE;
    int vSpace = 2;
    int topY = vSpace + bigTextFontH + TEXTLINESPACE;
    int bottomY = height() - vSpace - curFontMetrics.height() - SLIDERLEAVESPACE;
    int graduationLength = (bottomY - topY) * 0.5;
    int midLineLength = (bottomY - topY) * 0.8;
    double pixgraduation = (width() - 2 * hSpace) / (double)lineCount;
    hStartPos_ = hSpace;
    hEndXPos_ = hSpace + (lineCount - 1) * pixgraduation;

    QVector<QLine> bigLines,midLines,graduationLines;
    for(int i = 0; i < lineCount; i++){
        QLine line;
        int x1,y1,x2,y2;
        x1 = hSpace + static_cast<int>(i * pixgraduation);
        x2 = x1;
        y2 = bottomY;
        double value = i * graduation();
        if(i % 5 == 0){
            QString valueStr = gerateString_(value);
            if((i / 5) % 2 == 0){
                y1 = topY;
                line.setLine(x1,y1,x2,y2);
                bigLines << line;
                painter.setPen(bigTextPen_);
                painter.setFont(bigTextFont());
                painter.drawText(x1 - bigFontMetrics.width(valueStr) / 2,y1 - TEXTLINESPACE,valueStr);
            }else{
                y1 = bottomY - midLineLength;
                line.setLine(x1,y1,x2,y2);
                midLines << line;
                painter.setPen(midTextPen_);
                painter.setFont(midTextFont());
                painter.drawText(x1 - midFontMetrics.width(valueStr) / 2,y1 - TEXTLINESPACE,valueStr);
            }
        }else{
            y1 = bottomY - graduationLength;
            line.setLine(x1,y1,x2,y2);
            graduationLines << line;
        }
    }
    painter.setPen(bigValueLinePen_);
    painter.drawLines(bigLines);
    painter.setPen(midValueLinePen_);
    painter.drawLines(midLines);
    painter.setPen(graduationLinePen_);
    painter.drawLines(graduationLines);
    QLine bottomLine(QPoint(0,bottomY),QPoint(width(),bottomY));
    painter.setPen(bottomLinePen_);
    painter.drawLine(bottomLine);

    if(!sliderRectInited_){
        pixValue_ = qAbs(hEndXPos_ - hStartPos_) / (maxValue() - minValue());
        sliderRect_.setWidth(SLIEDERNORMALWIDTH);
        sliderRect_.setHeight(height() - vSpace - curFontMetrics.height());
        sliderRectInited_ = true;
    }
    if(value() >= maxValue()){
        setValue(maxValue());
    }
    sliderRect_.moveLeft(hSpace + value() * pixValue_ - sliderRect_.width() / 2);
//    QPalette pal;
//    pal.setColor(QPalette::Light,Qt::green);
//    pal.setColor(QPalette::Dark,Qt::yellow);
//    pal.setColor(QPalette::Mid,Qt::blue);
//    pal.setColor(QPalette::Base,Qt::red);
    painter.setBrush(Qt::yellow);
    painter.setPen(QPen(Qt::yellow,1));
    painter.drawRect(sliderRect_);
//    qDrawShadePanel(&painter,sliderRect_,pal,sliderIsDown_,1,&pal.brush(QPalette::Base));
    painter.setFont(curValueFont());
    painter.setPen(curTextPen_);
    double v = value();
    QString curTextStr = gerateString_(v);
    int curTextWidth = curFontMetrics.width(curTextStr);
    int curTextStartX = sliderRect_.center().x() - curTextWidth / 2;
    if(curTextStartX < hStartPos_){
        curTextStartX = hStartPos_;
    }
    if(curTextStartX + curTextWidth > hEndXPos_){
        curTextStartX = hEndXPos_ - curTextWidth;
    }
    painter.drawText(curTextStartX,height() - vSpace,curTextStr);
}

void TimeAxis::mouseMoveEvent(QMouseEvent *event)
{
    QCursor curCursor = cursor();
    if(sliderRect_.contains(event->pos())){
        if(!sliderRectIsBigState_){
            sliderRect_ = sliderRect_.adjusted(-SLIDERIN,0,SLIDERIN,0);
            sliderRectIsBigState_ = true;
            curCursor.setShape(Qt::PointingHandCursor);
            setCursor(Qt::PointingHandCursor);
        }
    }else{
        if(sliderRectIsBigState_){
            sliderRect_ = sliderRect_.adjusted(SLIDERIN,0,-SLIDERIN,0);
            sliderRectIsBigState_ = false;
            curCursor.setShape(Qt::PointingHandCursor);
            setCursor(Qt::ArrowCursor);
        }
    }
    if(sliderIsDown_){
        qreal offset = event->pos().x() - lastX_;
        lastX_ = event->pos().x();
        qreal xPos = sliderRect_.center().x() + offset;
        if(xPos < SLIDERHSPACE){
            xPos = hStartPos_;
        }else if(xPos > hEndXPos_){
            xPos = hEndXPos_;
        }
        sliderRect_.moveCenter(QPointF(xPos,sliderRect_.center().y()));
        setValue(minValue() + (xPos - hStartPos_) / pixValue_);
        emit valueChanged();
    }
    update();
}

void TimeAxis::mousePressEvent(QMouseEvent *event)
{
    if(sliderRect_.contains(event->pos())){
        lastX_ = event->pos().x();
        sliderIsDown_ = true;
    }
}

void TimeAxis::mouseReleaseEvent(QMouseEvent *event)
{
    sliderIsDown_ = false;
}

void TimeAxis::leaveEvent(QEvent *event)
{
    sliderIsDown_ = false;
}
