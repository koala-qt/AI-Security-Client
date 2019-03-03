#ifndef SCENEIMAGEDIALOG_H
#define SCENEIMAGEDIALOG_H

#include <QDialog>
#include "widgetinterface.h"
#include "service/servicei.h"
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(SelectImage)
QT_FORWARD_DECLARE_CLASS(RectsImage)
#define USERECTIMAGE
#define SINGLEIMAGE
class SceneImageDialog : public QDialog
{
    Q_OBJECT
public:
    SceneImageDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void setSceneInfo(const RestServiceI::SceneInfo &sinfo);
    void setRectLinePen(QColor);
    void setUserStyle(int);

signals:
    void sigImages(QVector<QImage>);

private:
    QVector<QImage> selectedImages_;
    QWidget *operateAreaW_{nullptr};
#ifdef USERECTIMAGE
    RectsImage *rectesImgArea_{nullptr};
#else
    SelectImage *selectAreaW_{nullptr};
#endif
    QListWidget *listW_{nullptr};
    QLabel *spiteL_{nullptr};
    QDialogButtonBox *btnBox_{nullptr};
    QSize itemSizeHint_;
    RestServiceI::SceneInfo curScenInfo_;
    QPushButton *searchBtn_{nullptr},*cancelBtn_{nullptr},*sureSelectBtn_{nullptr},*deleSelectBtn_{nullptr},*saveBtn_{nullptr};

private slots:
    void slotSaveBtnClicked();
    void slotSearchBtnClicked();
    void slotSureBtnClicked();
    void slotDeleteBtnClicke();
#ifdef USERECTIMAGE
    void slotOnClickedImage(QImage);
#endif
};

#endif // SCENEIMAGEDIALOG_H
