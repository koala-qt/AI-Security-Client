#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QTimer>

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = nullptr);

public slots:
    void slotSetPainterCoordinate(int,int);
    void slotSetErrorStr(QString);
    void slotSetPolygons(const QVector<QPair<QColor,QPolygonF>> &points);
    void slotSetRects(QVector<QRect> &rs);
    void slotSetPosition(QString);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_width = -1,m_height = -1;
    bool m_isBox = false;
    QTimer m_tm;
    QVector<QPair<QColor, QPolygonF> > m_listPoints;
    QVector<QRect> m_vecRects;
    QString m_errorStr,m_posStr;

private slots:
    void slotTimeout();
};

#endif // CANVASWIDGET_H
