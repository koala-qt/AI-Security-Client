#ifndef SCENEIMAGEDIALOG_H
#define SCENEIMAGEDIALOG_H

#include <QDialog>=
#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(SelectImage)
class SceneImageDialog : public QDialog
{
    Q_OBJECT
public:
    SceneImageDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void setImage(QImage img);
    void setRectLinePen(QColor);
    void setUserStyle(int);

signals:
    void sigImages(QVector<QImage>);

private:
    QVector<QImage> selectedImages_;
    WidgetManagerI *widgetM_{nullptr};
    QWidget *operateAreaW_{nullptr};
    SelectImage *selectAreaW_{nullptr};
    QListWidget *listW_{nullptr};
    QLabel *spiteL_{nullptr};
    QDialogButtonBox *btnBox_{nullptr};
    QSize itemSizeHint_;
    QPushButton *searchBtn_{nullptr},*cancelBtn_{nullptr},*sureSelectBtn_{nullptr},*deleSelectBtn_{nullptr};

private slots:
    void slotSearchBtnClicked();
    void slotSureBtnClicked();
    void slotDeleteBtnClicke();
};

#endif // SCENEIMAGEDIALOG_H
