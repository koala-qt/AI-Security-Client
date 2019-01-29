#ifndef EVENTCALENDER_H
#define EVENTCALENDER_H

#include <QCalendarWidget>
class EventCalender : public QCalendarWidget
{
    Q_OBJECT
public:
    EventCalender(QWidget *parent = nullptr);
    void setMaxValue(const int);
    int maxValue() const;
    void setEventBackColor(const QColor &c);
    QColor eventBackColor() const;

protected:
#if 1
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;
#endif

private:
    QHash<QDate,int> dataHash_;
    int maxValue_;
    QColor c_;
};

#endif // EVENTCALENDER_H
