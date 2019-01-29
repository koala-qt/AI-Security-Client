#ifndef CORNERWIDGET_H
#define CORNERWIDGET_H

#include <QWidget>
class CornerWidget : public QWidget
{
    Q_OBJECT
public:
    CornerWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // CORNERWIDGET_H
