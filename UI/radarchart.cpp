#include <QPainter>
#include <QtMath>
#include <QDebug>
#include "radarchart.h"

RadarChart::RadarChart(RadarType t, QWidget *parent) :
    type_(t),
    QWidget(parent)
{

}

void RadarChart::setTitle(const QString str, const Qt::AlignmentFlag f)
{
    title_ = str;
    titleAlign_ = f;
}

QString RadarChart::title() const
{
    return title_;
}

void RadarChart::setAxiesX(const QVector<QString> &v)
{
    axiesX_ = v;
}

void RadarChart::setState(const QString &s)
{
    stateStr_ = s;
}

QString RadarChart::state() const
{
    return stateStr_;
}

QVector<QString> RadarChart::axiesX() const
{
    return axiesX_;
}

void RadarChart::setYxiesRange(const double min, const double max, const double step)
{
    yMin_ = min;
    yMax_ = max;
    yStep_ = step;
}

void RadarChart::setXAxiesLabelVisible(const bool s)
{
    axiesLabelVisible_ = s;
}

double RadarChart::yMin() const
{
    return yMin_;
}

double RadarChart::yMax() const
{
    return yMax_;
}

double RadarChart::yStep() const
{
    return yStep_;
}

void RadarChart::clear()
{
    datas_.clear();
}

void RadarChart::append(const QString &label, QColor &color, const QVector<double> &data)
{
    datas_.append(std::make_tuple(label,color,data));
}

void RadarChart::updateValue(const QString &label, const QVector<double> &data)
{
    for(auto &value : datas_) {
        if(std::get<0>(value) == label){
            std::get<2>(value) = data;
            break;
        }
    }
}

QVector<std::tuple<QString,QColor,QVector<double>>> RadarChart::data() const
{
    return datas_;
}

void RadarChart::setAllFont(const QFont &titleFont, const QFont &axiesXFont, const QFont &axiesYFont, const QFont &valueLabelFont, const QFont &stateLabelFont)
{
    titleFont_ = titleFont;
    axiesXFont_ = axiesXFont;
    axiesYFont_ = axiesYFont;
    valueLabelFont_ = valueLabelFont;
    stateLabelFont_ = stateLabelFont;
}

void RadarChart::setAllPen(const QPen &titlePen,const QPen &gridPen, const QPen &axiesXPen, const QPen &axiesYPen, const QPen &valueLabelPen, const QPen &stateLabelPen)
{
    titlePen_ = titlePen;
    gridPen_ = gridPen;
    axiesXPen_ = axiesXPen;
    axiesYPen_ = axiesYPen;
    valueLabelPen_ = valueLabelPen;
    stateLabelPen_ = stateLabelPen;
}

void RadarChart::setBackground(const QImage &c, const int radius)
{
    backImg_ = c;
    radius_ = radius;
}

int RadarChart::backgroundRadius() const
{
    return radius_;
}

Qt::AlignmentFlag RadarChart::titleAlign() const
{
    return titleAlign_;
}

QFont RadarChart::titleFont() const
{
    return titleFont_;
}

QFont RadarChart::axiesXFont() const
{
    return axiesXFont_;
}

QFont RadarChart::axiesYFont() const
{
    return axiesYFont_;
}

QFont RadarChart::valueLabelFont() const
{
    return valueLabelFont_;
}

QFont RadarChart::stateLabelFont() const
{
    return stateLabelFont_;
}

QPen RadarChart::titlePen() const
{
    return titlePen_;
}

QPen RadarChart::axiesXPen() const
{
    return axiesXPen_;
}

QPen RadarChart::axiesYPen() const
{
    return axiesYPen_;
}

QPen RadarChart::gridPen() const
{
    return gridPen_;
}

QPen RadarChart::valueLabelPen() const
{
    return valueLabelPen_;
}

QPen RadarChart::stateLabelPen() const
{
    return stateLabelPen_;
}

QSize RadarChart::sizeHint() const
{
    return QSize(464,304);
}

void RadarChart::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QPixmap::fromImage(backImg_));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),radius_,radius_);
    if(datas_.isEmpty() || axiesX_.isEmpty())return;
    QFontMetrics titleMetrics(titleFont_);
    QRect titleTextRect(QPoint(RACHARTHSPACE,RACHARTVSPACE),QPoint(width() - titlePen_.width() - RACHARTHSPACE,RACHARTVSPACE + titleMetrics.height()));
    painter.setFont(titleFont_);
    painter.setPen(titlePen_);
    painter.drawText(titleTextRect,titleAlign_,title_);

    QRect subRect;
    QFontMetrics valueLabelMetrics(valueLabelFont_),axiesXMetrics(axiesXFont_),stateMetrics(stateLabelFont_);
    int legendH = (datas_.count() - 1) * VALUELABELVSPACE + valueLabelMetrics.height() * datas_.count();
    int legendX = RACHARTHSPACE;
    std::vector<int> legendLabelVec;
    QVector<QPair<QString,QColor>> labes;
    foreach (auto &value, datas_) {
        legendLabelVec.push_back(valueLabelMetrics.width(std::get<0>(value)));
        labes << qMakePair(std::get<0>(value),std::get<1>(value));
    }
    int maxLegendLabelW = *std::max_element(legendLabelVec.begin(),legendLabelVec.end());
    switch (layoutDirection()) {
    case Qt::LeftToRight:
        legendX = RACHARTHSPACE;
        if(axiesLabelVisible_){
            subRect.setLeft(legendX + maxLegendLabelW + VALUELABELHSPACE + LABELPOINTSIZE / 2 + MAXAXIESLABELWIDTH + AXIESXLABELSPACE);
            subRect.setRight(width() - RACHARTHSPACE - MAXAXIESLABELWIDTH - AXIESXLABELSPACE);
        }else{
            subRect.setLeft(legendX + maxLegendLabelW + VALUELABELHSPACE + LABELPOINTSIZE / 2);
            subRect.setRight(width() - RACHARTHSPACE);
        }
        subRect.setTop(titleTextRect.bottom() + axiesXMetrics.height() + TITLE2FISTXAXIESSPACE + AXIESXLABELSPACE);
        if(stateStr_.isEmpty()){
            if(axiesX_.count() % 2){
                subRect.setBottom(height() - RACHARTVSPACE);
            }else{
                subRect.setBottom(height() - axiesXMetrics.height() - RACHARTVSPACE);
            }
        }else{
            if(axiesX_.count() % 2){
                subRect.setBottom(height() - stateMetrics.height() - RACHARTVSPACE);
            }else{
                subRect.setBottom(height() - axiesXMetrics.height() - stateMetrics.height() - RACHARTVSPACE);
            }
        }
        break;
    case Qt::RightToLeft:
        legendX = width() - RACHARTHSPACE - VALUELABELHSPACE - maxLegendLabelW - LABELPOINTSIZE / 2;
        if(axiesLabelVisible_){
            subRect.setLeft(RACHARTHSPACE + MAXAXIESLABELWIDTH + AXIESXLABELSPACE);
            subRect.setRight(legendX - LABELPOINTSIZE - MAXAXIESLABELWIDTH - AXIESXLABELSPACE);
        }else{
            subRect.setLeft(RACHARTHSPACE);
            subRect.setRight(legendX - LABELPOINTSIZE);
        }
        subRect.setTop(titleTextRect.bottom() + axiesXMetrics.height() + TITLE2FISTXAXIESSPACE + AXIESXLABELSPACE);
        if(stateStr_.isEmpty()){
            if(axiesX_.count() % 2){
                subRect.setBottom(height() - RACHARTVSPACE);
            }else{
                subRect.setBottom(height() - axiesXMetrics.height() - RACHARTVSPACE);
            }
        }else{
            if(axiesX_.count() % 2){
                subRect.setBottom(height() - stateMetrics.height() - RACHARTVSPACE);
            }else{
                subRect.setBottom(height() - axiesXMetrics.height() - stateMetrics.height() - RACHARTVSPACE);
            }
        }
        break;
    default:
        break;
    }

    qreal spliderRadius = qMin(subRect.width(),subRect.height()) / 2;
    qreal dRadian = PI * 2 / axiesX_.count();
    int tickCount = qAbs(yMax_ - yMin_) / qAbs(yStep_);
    qreal pixStep = spliderRadius / tickCount;
    qreal x0 = subRect.center().x();
    qreal y0 = subRect.center().y();
    QVector<QLineF> yAxiesLines;
    for(int i = 0; i < tickCount + 1; i++){
        qreal curRadius = pixStep * i;
        QPolygonF polyLines;
        for(int j = 0; j < axiesX_.count(); j++){
            QPointF curPf(x0 + curRadius*::qSin(dRadian*j),y0 - curRadius*qCos(dRadian*j));
            polyLines << curPf;
            if(j == 0){
                painter.setPen(axiesYPen_);
                painter.setFont(axiesYFont_);
                painter.drawText(curPf, QString::number(yMin_ + i * yStep_));
            }
            if(i == tickCount){
                yAxiesLines << QLineF(QPointF(x0,y0),curPf);
                painter.setPen(axiesXPen_);
                painter.setFont(axiesXFont_);
                QRect labelRect;
                labelRect.setSize(QSize(MAXAXIESLABELWIDTH,axiesXMetrics.height()));
                labelRect.moveCenter(curPf.toPoint());
                if(qAbs(curPf.x() - x0) < 6e-6){
                    labelRect.moveLeft(curPf.x() - labelRect.width() / 2);
                    if(curPf.y() < y0){
                        labelRect.moveBottom(curPf.y() - labelRect.height());
                    }else{
                        labelRect.moveBottom(curPf.y() + labelRect.height());
                    }
                }else{
                    if(curPf.x() < x0){
                        labelRect.moveLeft(curPf.x() - labelRect.width() - AXIESXLABELSPACE);
                    }else{
                        labelRect.moveLeft(curPf.x() + AXIESXLABELSPACE);
                    }
                }
                if(axiesLabelVisible_){
                    painter.drawText(labelRect,Qt::AlignLeft,axiesX_.at(j));
                }
            }
        }
        if(!polyLines.count()){
            continue;
        }
        polyLines << polyLines.first();
        painter.setPen(gridPen_);
        painter.drawPolyline(polyLines);
    }
    painter.setPen(axiesYPen_);
    painter.drawLines(yAxiesLines);

    painter.setFont(valueLabelFont_);
    qreal pixPer = spliderRadius / qAbs(yMax_ - yMin_);
    for(int i = 0; i < datas_.count(); i++){
        const QVector<double> &values = std::get<2>(datas_.at(i));
        QColor c = std::get<1>(datas_.at(i));
        QPolygonF polyLines;
        for(int j = 0; j < values.count(); j++){
            qreal curRadius = pixPer * (values.at(j) - yMin_);
            polyLines << QPointF(x0 + curRadius*::qSin(dRadian*j),y0 - curRadius*qCos(dRadian*j));
        }

        painter.setPen(c);
        painter.setBrush(c);
        painter.drawPolygon(polyLines);

        int textY = height() - legendH + i * (valueLabelMetrics.height() + VALUELABELVSPACE);
        c.setAlpha(255);
        painter.setPen(QPen(c,LABELPOINTSIZE));
        painter.drawPoint(legendX,textY - valueLabelMetrics.height() / 2);
        painter.setPen(valueLabelPen_);
        painter.drawText(legendX + VALUELABELHSPACE,height() - legendH + i * (valueLabelMetrics.height() + VALUELABELVSPACE),std::get<0>(datas_.at(i)));
    }

    painter.setPen(stateLabelPen_);
    painter.setFont(stateLabelFont_);
    painter.drawText(x0 - stateMetrics.width(stateStr_) / 2,height() - RACHARTVSPACE,stateStr_);

    return QWidget::paintEvent(event);
}
