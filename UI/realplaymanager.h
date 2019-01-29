#ifndef REALPLAYMANAGER_H
#define REALPLAYMANAGER_H

#include "widgetinterface.h"
#include <QStack>
#include <QMap>
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QFocusFrame)
QT_FORWARD_DECLARE_CLASS(VideoPlayer)
class RealPlayManager : public WidgetI
{
    Q_OBJECT
public:
    explicit RealPlayManager(WidgetManagerI*wm,WidgetI*parent = nullptr);
    ~RealPlayManager();
    void setUserStyle(WidgetManagerI::SkinStyle) override;
    int screenCount()const;
    void splitScreen(const int rows, const int cols, const int bigFrameRow, const int bigFrameCol, const int bigFrameRowSpan, const int bigFrameColSpan);
    void playByOrder(QString url, QString id, QString name = QString());
    void playByFocus(QString url, QString id,QString name = QString());
    void playByPosition(QString url, QString id, int pos);
    VideoPlayer* focusPlayer() const;
    int frameCount() const;

signals:
    void sigVideoStarted(QString);
    void sigVideoRemoved(QString);
    void sigVideoError(QString);

protected:
    void mouseDoubleClickEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

private:
    QGridLayout *m_mainLay{nullptr};
    bool m_isFullScreen = false;
    QFocusFrame *m_focusFrame{nullptr};
    QMap<QString,VideoPlayer*> m_videoMap;
    int curRow = 0,curCol = 0,curBigRow = 0,curBigCol = 0,curBigRowSpan = 0,curBigColSpan = 0;
    QString decoderName_;

    void videoShowFullScreen(QWidget *);
    void closeVideoFullScreen();

private slots:
    void slotDectecBoxes(QPair<QString, QVector<QRect> >);
    void slotVideoStarted(int, int);
    void slotVideoError(QString);
};

#endif // REALPLAYMANAGER_H
