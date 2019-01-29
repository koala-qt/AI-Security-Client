#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>
class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ButtonDelegate(QObject *parent = nullptr);
    void setText(QString);
    void setPalette(QPalette);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)override;

private:
    QString text_;
    QPalette pal_;
    QPoint mousePoint_;
    int buttonType_ = 0;
};

#endif // BUTTONDELEGATE_H
