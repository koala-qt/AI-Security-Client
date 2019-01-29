#ifndef RECTNUMBERWIDGET_H
#define RECTNUMBERWIDGET_H

#include <QWidget>
#include <QPen>
#define VSPACE 5
const float g_shortborderper = 0.186;
class RectNumberWidget : public QWidget
{
    Q_OBJECT
public:
    RectNumberWidget(QWidget *parent = nullptr);
    void setValue(const QString);
    void setTitle(const QString,const Qt::AlignmentFlag);
    void setAllPen(const QPen &borderPen,const QPen &pointPen,const QPen &valuePen,const QPen &titlePen);
    void setAllFont(const QFont &valueFont,const QFont &titleFont);
    QPen borderPen() const;
    QPen pointPen() const;
    QPen valuePen() const;
    QPen titlePen() const;
    QFont valueFont() const;
    QFont titleFont() const;
    QString title() const;
    Qt::AlignmentFlag titleAlign() const;
    QString value() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPen borderPen_,pointPen_,valuePen_,titletPen_;
    QString title_,value_;
    Qt::AlignmentFlag titleAlign_;
    QFont valueFont_,titleFont_;
};

#endif // RECTNUMBERWIDGET_H
