#ifndef WAITINGLABEL_H
#define WAITINGLABEL_H

#include <QLabel>
QT_FORWARD_DECLARE_CLASS(QMovie)
class WaitingLabel : public QLabel
{
    Q_OBJECT
public:
    WaitingLabel(QWidget *parent = nullptr);
    void setMovieFile(QString);

public slots:
    void show(int timeout);

private:
    QMovie *movie_{nullptr};
};

#endif // WAITINGLABEL_H
