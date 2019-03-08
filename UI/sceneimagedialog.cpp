#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScrollBar>
#include <QApplication>
#include <QMenu>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include "sceneimagedialog.h"
#include "facesearch.h"
#include "components/SelectImage/selectimage.h"
#include "components/RectsImage/rectsimage.h"
#include "informationdialog.h"

SceneImageDialog::SceneImageDialog(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
#ifdef USERECTIMAGE
    rectesImgArea_ = new RectsImage;
#else
    selectAreaW_ = new SelectImage;
#endif
    spiteL_ = new QLabel;
    listW_ = new QListWidget;
    btnBox_ = new QDialogButtonBox;
    sureSelectBtn_ = new QPushButton(tr("ok"));
    deleSelectBtn_ = new QPushButton(tr("delete all"));
    operateAreaW_ = new QWidget;
#ifdef USERECTIMAGE
    saveBtn_ = new QPushButton(tr("Save images"),rectesImgArea_);
#else
    saveBtn_ = new QPushButton(tr("Save scene"),selectAreaW_);
#endif
    QVBoxLayout *vlay = new QVBoxLayout;
#ifdef USERECTIMAGE
    vlay->addWidget(rectesImgArea_);
#else
    vlay->addWidget(selectAreaW_);
#endif
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(sureSelectBtn_);
    hlay->addWidget(deleSelectBtn_);
    hlay->setAlignment(Qt::AlignRight);
    vlay->addLayout(hlay);
    vlay->setMargin(0);

    hlay = new QHBoxLayout;
    hlay->addLayout(vlay,9);
    hlay->addWidget(spiteL_);
    hlay->addWidget(listW_,2);
    hlay->setContentsMargins(10,10,10,10);
    operateAreaW_->setLayout(hlay);
    mainLay->addWidget(operateAreaW_);
    btnBox_->setContentsMargins(0,0,10,0);
    mainLay->addWidget(btnBox_);
    mainLay->setContentsMargins(0,0,0,10);
    setLayout(mainLay);

    cancelBtn_ = btnBox_->addButton(tr("Cancel"),QDialogButtonBox::RejectRole);
    searchBtn_ = btnBox_->addButton(tr("Search"),QDialogButtonBox::AcceptRole);
    cancelBtn_->setFocusPolicy(Qt::NoFocus);
    searchBtn_->setFocusPolicy(Qt::NoFocus);
    searchBtn_->setDefault(false);
    saveBtn_->setFocusPolicy(Qt::NoFocus);
    sureSelectBtn_->setFocusPolicy(Qt::NoFocus);
    deleSelectBtn_->setFocusPolicy(Qt::NoFocus);
    cancelBtn_->setFixedSize(120,44);
    searchBtn_->setFixedSize(120,44);
    sureSelectBtn_->setIcon(QPixmap("images/btn_sure.jpg"));
    deleSelectBtn_->setIcon(QPixmap("images/delet_all_imgs.png"));
    spiteL_->setFixedWidth(2);
    listW_->setViewMode(QListWidget::IconMode);
    itemSizeHint_ = QSize(124,124);
    listW_->setIconSize(QSize(119,119));
    listW_->setResizeMode(QListView::Adjust);
    listW_->setFrameStyle(QFrame::NoFrame);
    listW_->setSpacing(0);
    int listWidth = itemSizeHint_.width() * 2 + listW_->frameWidth() * 2 + listW_->spacing() * 3 +
            style()->pixelMetric(QStyle::PM_ScrollBarSliderMin);
    listW_->setFixedWidth(listWidth);
    spiteL_->setFixedWidth(1);

    connect(sureSelectBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSureBtnClicked()));
    connect(deleSelectBtn_,SIGNAL(clicked(bool)),this,SLOT(slotDeleteBtnClicke()));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(cancelBtn_,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(saveBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSaveBtnClicked()));
#ifdef USERECTIMAGE
    sureSelectBtn_->hide();
    listW_->hide();
    spiteL_->hide();
    deleSelectBtn_->hide();
    setRectLinePen(QColor(19,255,175));
    connect(rectesImgArea_,SIGNAL(sigClickedImage(QImage)),this,SLOT(slotOnClickedImage(QImage)));
#endif
}

void SceneImageDialog::setShowRect(bool face, bool body)
{
    rectesImgArea_->setShowRect(face,body);
}

void SceneImageDialog::setSceneInfo(const RestServiceI::SceneInfo &sinfo)
{
#ifdef USERECTIMAGE
    curScenInfo_ = sinfo;
    rectesImgArea_->setInfos(curScenInfo_.image,sinfo.faceRectVec,sinfo.bodyRectVec);
    if(sinfo.faceRectVec.isEmpty()){
        searchBtn_->hide();
    }
#else
    selectAreaW_->setBackImage(sinfo.image);
#endif
}

void SceneImageDialog::setRectLinePen(QColor c, QColor b)
{
#ifdef USERECTIMAGE
    rectesImgArea_->setRectLineColor(c,b);
#else
    QPalette pal = selectAreaW_->palette();
    pal.setColor(QPalette::Foreground,c);
    selectAreaW_->setPalette(pal);
#endif
}

void SceneImageDialog::setUserStyle(int styleArg)
{
    QPalette pal;
    if(styleArg == 0){
        pal = palette();
        pal.setColor(QPalette::Background,QColor(48,54,68));
        setPalette(pal);

        pal = operateAreaW_->palette();
        pal.setColor(QPalette::Background,QColor(48,54,68));
        operateAreaW_->setPalette(pal);
        operateAreaW_->setAutoFillBackground(true);

        pal = spiteL_->palette();
        pal.setColor(QPalette::Background,QColor(255,255,255,150));
        spiteL_->setPalette(pal);
        spiteL_->setAutoFillBackground(true);

        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 4px;"
                                  "font-size: 12px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        cancelBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 4px;"
                                  "font-size: 12px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        sureSelectBtn_->setStyleSheet("QPushButton{"
                                      "background-color: rgb(83,77,251);"
                                      "color: white;"
                                      "border-radius: 6px;"
                                      "font-size: 18px;"
                                      "}"
                                      "QPushButton:pressed{"
                                      "padding: 2px;"
                                      "background-color: #312DA6;"
                                      "}");
        deleSelectBtn_->setStyleSheet("QPushButton{"
                                      "background-color: rgb(83,77,251);"
                                      "color: white;"
                                      "border-radius: 6px;"
                                      "font-size: 18px;"
                                      "}"
                                      "QPushButton:pressed{"
                                      "padding: 2px;"
                                      "background-color: #312DA6;"
                                      "}");
        saveBtn_->setStyleSheet("QPushButton{"
                                "background-color: rgb(83,77,251);"
                                "color: white;"
                                "}"
                                "QPushButton:pressed{"
                                "padding: 2px;"
                                "background-color: #312DA6;"
                                "}");

        pal = listW_->palette();
        pal.setColor(QPalette::Base,Qt::transparent);
        listW_->setPalette(pal);
        listW_->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                                    "background: transparent;"
                                                    "border: none;"
                                                    "}"
                                                    "QScrollBar::handle:vertical{"
                                                    "background: rgb(100,100,100);"
                                                    "border-radius: 5px;"
                                                    "}"
                                                    "QScrollBar::add-line:vertical{"
                                                    "background: transparent;"
                                                    "height: 0px;"
                                                    "}"
                                                    "QScrollBar::sub-line:vertical{"
                                                    "background: transparent;"
                                                    "height: 0px;"
                                                    "}"
                                                    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                                    "background: transparent;"
                                                    "}");
    }
}

void SceneImageDialog::slotSaveBtnClicked()
{
    QString filePath =  QFileDialog::getExistingDirectory(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),QFileDialog::DontResolveSymlinks);
    if(filePath.isEmpty())return;
    for(int i = 0; i < curScenInfo_.faceRectVec.count(); i++){
        if(!curScenInfo_.faceRectVec.at(i).second.save(filePath + tr("/%1-face-%2.jpg").arg(curScenInfo_.sceneId).arg(i))){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(0);
            infoDialog.setMessage("Operation failed!");
        }
    }
    for(int i = 0; i < curScenInfo_.bodyRectVec.count(); i++){
        if(!curScenInfo_.bodyRectVec.at(i).second.save(filePath + tr("/%1-body-%2.jpg").arg(curScenInfo_.sceneId).arg(i))){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(0);
            infoDialog.setMessage("Operation failed!");
        }
    }
}

void SceneImageDialog::slotSearchBtnClicked()
{
    emit sigImages(selectedImages_);
}

void SceneImageDialog::slotSureBtnClicked()
{
#ifdef USERECTIMAGE
#else
    listW_->clear();
    selectedImages_ = selectAreaW_->selectedImages();
    for(QImage &img : selectedImages_){
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole,QPixmap::fromImage(img));
        item->setSizeHint(itemSizeHint_);
        item->setIcon(QPixmap::fromImage(img.scaled(listW_->iconSize())));
        item->setTextAlignment(Qt::AlignCenter);
        listW_->addItem(item);
    }
    selectAreaW_->clearImages();
#endif
}

void SceneImageDialog::slotDeleteBtnClicke()
{
#ifdef USERECTIMAGE
    listW_->clear();
    selectedImages_.clear();
#else
    selectAreaW_->clearImages();
#endif
}

#ifdef USERECTIMAGE
void SceneImageDialog::slotOnClickedImage(QImage img)
{
#if 0
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::UserRole,QPixmap::fromImage(img));
    item->setSizeHint(itemSizeHint_);
    item->setIcon(QPixmap::fromImage(img.scaled(listW_->iconSize())));
    item->setTextAlignment(Qt::AlignCenter);
    listW_->addItem(item);
    selectedImages_ << img;
#endif
    selectedImages_.clear();
    selectedImages_ << img;
}
#endif
