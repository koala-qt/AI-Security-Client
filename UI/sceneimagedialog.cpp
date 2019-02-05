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
#include <QDir>
#include "sceneimagedialog.h"
#include "facesearch.h"
#include "components/SelectImage/selectimage.h"

SceneImageDialog::SceneImageDialog(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    selectAreaW_ = new SelectImage;
    spiteL_ = new QLabel;
    listW_ = new QListWidget;
    btnBox_ = new QDialogButtonBox;
    sureSelectBtn_ = new QPushButton(tr("ok"));
    deleSelectBtn_ = new QPushButton(tr("delete all"));
    operateAreaW_ = new QWidget;
    saveBtn_ = new QPushButton(tr("Save scene"),operateAreaW_);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(selectAreaW_);
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
    hlay->setContentsMargins(10,10,0,10);
    hlay->setSpacing(20);
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

    widgetM_ = reinterpret_cast<WidgetManagerI*>(qApp->property("WorkerManager").toULongLong());
    connect(sureSelectBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSureBtnClicked()));
    connect(deleSelectBtn_,SIGNAL(clicked(bool)),this,SLOT(slotDeleteBtnClicke()));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(cancelBtn_,SIGNAL(clicked(bool)),btnBox_,SIGNAL(rejected()));
    connect(saveBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSaveBtnClicked()));
}

void SceneImageDialog::setImage(QImage img)
{
    selectAreaW_->setBackImage(img);
}

void SceneImageDialog::setImage(QImage img, QString sceneId)
{
    selectAreaW_->setBackImage(img);
    curImage_ = img;
    curSceneId_ = sceneId;
}

void SceneImageDialog::setRectLinePen(QColor c)
{
    QPalette pal = selectAreaW_->palette();
    pal.setColor(QPalette::Foreground,c);
    selectAreaW_->setPalette(pal);
}

void SceneImageDialog::setUserStyle(int styleArg)
{
    QPalette pal;
    if(styleArg == 1){
        pal = palette();
        pal.setColor(QPalette::Background,QColor(87,87,87));
        setPalette(pal);

        pal = operateAreaW_->palette();
        pal.setColor(QPalette::Background,QColor(75,75,75));
        operateAreaW_->setPalette(pal);
        operateAreaW_->setAutoFillBackground(true);

        pal = spiteL_->palette();
        pal.setColor(QPalette::Background,QColor(255,255,255,150));
        spiteL_->setPalette(pal);
        spiteL_->setAutoFillBackground(true);

        searchBtn_->setStyleSheet("QPushButton{"
                                  "color: white;"
                                  "background-color: transparent;"
                                  "border: 1px solid rgba(255,255,255,0.4);"
                                  "border-radius: 20px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "background-color: rgb(180,160,108);"
                                  "}");
        cancelBtn_->setStyleSheet("QPushButton{"
                                  "color: white;"
                                  "background-color: transparent;"
                                  "border: 1px solid rgba(255,255,255,0.4);"
                                  "border-radius: 20px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "background-color: rgb(180,160,108);"
                                  "}");
        sureSelectBtn_->setStyleSheet("QPushButton{"
                                      "background-color: rgb(100,100,100);"
                                      "}");
        deleSelectBtn_->setStyleSheet("QPushButton{"
                                      "background-color: rgb(100,100,100);"
                                      "}");
        saveBtn_->setStyleSheet("QPushButton{"
                                      "background-color: rgb(100,100,100);"
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
    QDir usrDir("user/image");
    if(!usrDir.exists()){
        usrDir.mkpath(usrDir.path());
    }
    curImage_.save(usrDir.path() + "/" + curSceneId_ + ".jpg");
}

void SceneImageDialog::slotSearchBtnClicked()
{
    emit sigImages(selectedImages_);
}

void SceneImageDialog::slotSureBtnClicked()
{
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
}

void SceneImageDialog::slotDeleteBtnClicke()
{
    selectAreaW_->clearImages();
}
