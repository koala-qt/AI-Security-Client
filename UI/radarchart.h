#ifndef RADARCHART_H
#define RADARCHART_H

#include <QWidget>
#include <QMap>
#include <QPen>
#define PI 3.14159265f
#define RACHARTHSPACE 30 //水平边距
#define RACHARTVSPACE 30 //垂直边距
#define VALUELABELVSPACE 15 //legend标签元素间距
#define VALUELABELHSPACE 12 //legend元素标签与点间距
#define LABELPOINTSIZE 8 //标签点的长度
#define AXIESXLABELSPACE 15 //x轴标签与网格点的距离
#define TITLE2FISTXAXIESSPACE 10 //标题与顶上第一个x轴标签的距离
#define MAXAXIESLABELWIDTH 80 //max x label width
class RadarChart : public QWidget
{
    Q_OBJECT
public:
    enum RadarType{
        Radar,//雷达图
        Spider //蛛蛛网图
    };
    explicit RadarChart(RadarType t, QWidget *parent = nullptr);
    void setTitle(const QString,const Qt::AlignmentFlag);
    QString title() const;
    void setAxiesX(const QVector<QString> &);
    void setState(const QString &);
    QString state() const;
    QVector<QString> axiesX() const;
    void setYxiesRange(const double min,const double max,const double step);
    void setXAxiesLabelVisible(const bool);
    double yMin() const;
    double yMax() const;
    double yStep() const;
    void clear();
    void append(const QString &label,QColor &color,const QVector<double> &data);
    void updateValue(const QString &label,const QVector<double> &data);
    QVector<std::tuple<QString,QColor,QVector<double>>> data() const;
    void setAllFont(const QFont &titleFont,const QFont &axiesXFont,const QFont &axiesYFont,const QFont &valueLabelFont,const QFont &stateLabelFont);
    void setAllPen(const QPen &titlePen,const QPen &gridPen,const QPen &axiesXPen,const QPen &axiesYPen,const QPen &valueLabelPen,const QPen &stateLabelPen);
    void setBackground(const QImage &,const int radius = 0);
    int backgroundRadius() const;
    Qt::AlignmentFlag titleAlign() const;
    QFont titleFont() const;
    QFont axiesXFont() const;
    QFont axiesYFont() const;
    QFont valueLabelFont() const;
    QFont stateLabelFont() const;
    QPen titlePen() const;
    QPen axiesXPen() const;
    QPen axiesYPen() const;
    QPen gridPen() const;
    QPen valueLabelPen() const;
    QPen stateLabelPen() const;
    QSize sizeHint() const override;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage backImg_;
    int radius_;
    RadarType type_;
    QString title_,stateStr_;
    QVector<QString> axiesX_;
    Qt::AlignmentFlag titleAlign_;
    bool axiesLabelVisible_ = true;
    QVector<std::tuple<QString,QColor,QVector<double>>> datas_;
    QFont titleFont_,axiesXFont_,axiesYFont_,valueLabelFont_,stateLabelFont_;
    QPen titlePen_,axiesXPen_,axiesYPen_,valueLabelPen_,stateLabelPen_,gridPen_;
    double yMin_,yMax_,yStep_;
};

#endif // RADARCHART_H
