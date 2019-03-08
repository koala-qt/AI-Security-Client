#include <QLabel>
#include <QListWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QMenu>
#include <QStandardPaths>
#include <QFileDialog>
#include <QScrollBar>
#include <QPainter>
#include "videoanalysisdata.h"
#include "personmark.h"
#include "informationdialog.h"
#include "facelinkpage.h"
#include "trackingpage.h"
#include "facesearch.h"
#include "portraitsearch.h"

VideoAnalysisData::VideoAnalysisData(WidgetI *parent):
    WidgetI(parent)
{
    titleTextL_ = new QLabel(tr("Analysis result"));
    countTextL_ = new QLabel;
    dataListW_ = new QListWidget;

    QVBoxLayout *mainLay = new QVBoxLayout;
    QVBoxLayout *topVLay = new QVBoxLayout;
    topVLay->addWidget(titleTextL_);
    topVLay->addWidget(countTextL_);
    topVLay->setAlignment(Qt::AlignLeft);
    topVLay->setContentsMargins(40,40,0,10);
    mainLay->addLayout(topVLay);
    mainLay->addWidget(dataListW_);
    dataListW_->setSpacing(20);
    mainLay->setContentsMargins(0,0,0,0);
    setLayout(mainLay);

    dataListW_->setFrameStyle(QFrame::NoFrame);
    setUserStyle(userStyle());
}

void VideoAnalysisData::setUserStyle(int s)
{
    QPalette pal;
    if(s == 0){
        pal = titleTextL_->palette();
        pal.setColor(QPalette::Foreground,QColor(126,140,177));
        titleTextL_->setPalette(pal);
        countTextL_->setPalette(pal);

        dataListW_->setStyleSheet("QListWidget{"
                                  "background: transparent;"
                                  "font: 11px;"
                                  "color: white;"
                                  "}");
        dataListW_->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                                       "background: transparent;"
                                                       "border: 0px solid gray;"
                                                       "width: 13px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical{"
                                                       "background: rgba(255,255,255,0.5);"
                                                       "border-radius: 5px;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical{"
                                                       "background: transparent;"
                                                       "border:0px solid #274168;"
                                                       "border-radius: 5px;"
                                                       "min-height: 10px;"
                                                       "width: 13px;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical{"
                                                       "background: transparent;"
                                                       "border:0px solid #274168;"
                                                       "min-height: 10px;"
                                                       "width: 13px;"
                                                       "}"
                                                       "QScrollBar::up-arrow:vertical{"
                                                       "subcontrol-origin: margin;"
                                                       "height: 0px;"
                                                       "border:0 0 0 0;"
                                                       "visible:false;"
                                                       "}"
                                                       "QScrollBar::down-arrow:vertical{"
                                                       "subcontrol-origin: margin;"
                                                       "height: 0px;"
                                                       "visible:false;"
                                                       "}"
                                                       "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                                       "background: transparent;"
                                                       "border: none;"
                                                       "border-radius: 0px;"
                                                       "}");
    }
}
#include <QDebug>
void VideoAnalysisData::slotVideoAnalysisData(QString personId, QImage img)
{
    QList<QListWidgetItem*> itemsList = dataListW_->findItems(personId,Qt::MatchFixedString);
    QListWidgetItem *item = nullptr;
    if(itemsList.isEmpty()){
        item = new QListWidgetItem;
        item->setText(personId);
        item->setForeground(Qt::transparent);
        dataListW_->addItem(item);
        PersonInfo *personInfoW = new PersonInfo;
        dataListW_->setItemWidget(item,personInfoW);
    }else{
        item = itemsList.first();
    }

    countTextL_->setText(tr("Alerady analyzed %1 persons").arg(dataListW_->count()));
    PersonInfo *itemW = qobject_cast<PersonInfo*>(dataListW_->itemWidget(item));
    itemW->addItem(personId,img);
    item->setSizeHint(QSize(dataListW_->width() - style()->pixelMetric(QStyle::PM_ScrollBarSliderMin) - dataListW_->spacing() * 2,itemW->dynamicHeight()));
}

PersonInfo::PersonInfo(WidgetI *parent):
    WidgetI(parent)
{
    headImgL_ = new QLabel;
    personOrderL_ = new QLabel;
    personImgCountL_ = new QLabel;
    imgListW_ = new QListWidget;
    QVBoxLayout *mainLay = new QVBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(headImgL_,0,0,2,1);
    gridLay->addWidget(personOrderL_,0,1,1,1);
    gridLay->addWidget(personImgCountL_,1,1,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    gridLay->setContentsMargins(20,30,0,12);
    mainLay->addLayout(gridLay);
    imgListW_->setContentsMargins(0,0,0,0);
    imgListW_->setSpacing(18);
    mainLay->addWidget(imgListW_);
    setLayout(mainLay);

    menu_ = new QMenu(imgListW_);
    menu_->addAction(tr("Register"),[this]{
        PersonMark markDialog;
        markDialog.setPhoto(imgListW_->currentItem()->data(Qt::UserRole).value<QImage>());
        QDialog::DialogCode returnCode = QDialog::DialogCode(markDialog.exec());
        if(returnCode == QDialog::Accepted){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(0);
            infoDialog.setMessage(tr("Successed"));
            infoDialog.exec();
        }
    });
    menu_->addAction(tr("Capture search"),[this]{
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(userStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(imgListW_->currentItem()->data(Qt::UserRole).value<QImage>());
//        faceDialog->setOid("");
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });

    menu_->addAction(tr("Portrait search"),[this]{
        PortraitSearch *portSearchDialog = new PortraitSearch(this);
        portSearchDialog->setAttribute(Qt::WA_DeleteOnClose);
        portSearchDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        portSearchDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = portSearchDialog->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        portSearchDialog->setPalette(pal);
        portSearchDialog->setAutoFillBackground(true);
        portSearchDialog->setUserStyle(userStyle());
        portSearchDialog->setMinimumSize(1250,710);
        portSearchDialog->slotAddImage(imgListW_->currentItem()->data(Qt::UserRole).value<QImage>());
//        portSearchDialog->layout()->setMargin(10);
//        portSearchDialog->setMinimumHeight(700);
        portSearchDialog->show();
    });

    menu_->addAction(tr("tracking"),[this]{
        TrackingPage *view = new TrackingPage(this);
        QPalette pal = view->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        view->setPalette(pal);
        view->setAutoFillBackground(true);
        view->setUserStyle(userStyle());
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setWindowFlags(Qt::Window | Qt::Dialog);
        view->setWindowModality(Qt::ApplicationModal);
        view->setMinimumSize(1655,924);
        view->setImgageOid(imgListW_->currentItem()->data(Qt::UserRole).value<QImage>(),QString());
        view->show();
    });

    menu_->addAction(tr("face link"),[this]{
        FaceLinkPage *faceLinkP = new FaceLinkPage(this);
        QPalette pal = faceLinkP->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        faceLinkP->setPalette(pal);
        faceLinkP->setAutoFillBackground(true);
        faceLinkP->setAttribute(Qt::WA_DeleteOnClose);
        faceLinkP->setWindowFlags(Qt::Window | Qt::Dialog);
        faceLinkP->setWindowModality(Qt::ApplicationModal);
        faceLinkP->setFaceLinkOidAndImg("", QPixmap::fromImage(imgListW_->currentItem()->data(Qt::UserRole).value<QImage>()));
        faceLinkP->resize(1200,900);
        faceLinkP->show();
    });
    menu_->addAction(tr("save"),[this]{
        QString personId = imgListW_->currentItem()->text();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!imgListW_->currentItem()->data(Qt::UserRole).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    imgListW_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(imgListW_,&QListWidget::customContextMenuRequested,imgListW_,[this](const QPoint p){
        menu_->move(imgListW_->mapToGlobal(p));
        menu_->show();
    });
    imgListW_->setFlow(QListWidget::LeftToRight);
    imgListW_->setMovement(QListWidget::Static);
    imgListW_->setViewMode(QListWidget::IconMode);
    imgListW_->setResizeMode(QListWidget::Adjust);
    imgListW_->setIconSize(QSize(112,112));
    imgListW_->setFrameStyle(QFrame::NoFrame);
    imgListW_->installEventFilter(this);
    setUserStyle(userStyle());
}

void PersonInfo::setUserStyle(int s)
{
    if(s == 0){
        personOrderL_->setStyleSheet("QLabel{"
                                     "color: rgb(126,140,177);"
                                     "}");
        personImgCountL_->setStyleSheet("QLabel{"
                                        "color: rgb(126,140,177);"
                                        "}");
        imgListW_->setStyleSheet("QListWidget{"
                                 "color: rgb(126,140,177);"
                                 "}");
        imgListW_->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                                      "background: transparent;"
                                                      "border: 0px solid gray;"
                                                      "width: 13px;"
                                                      "}"
                                                      "QScrollBar::handle:vertical{"
                                                      "background: rgba(255,255,255,0.5);"
                                                      "border-radius: 5px;"
                                                      "}"
                                                      "QScrollBar::add-line:vertical{"
                                                      "background: transparent;"
                                                      "border:0px solid #274168;"
                                                      "border-radius: 5px;"
                                                      "min-height: 10px;"
                                                      "width: 13px;"
                                                      "}"
                                                      "QScrollBar::sub-line:vertical{"
                                                      "background: transparent;"
                                                      "border:0px solid #274168;"
                                                      "min-height: 10px;"
                                                      "width: 13px;"
                                                      "}"
                                                      "QScrollBar::up-arrow:vertical{"
                                                      "subcontrol-origin: margin;"
                                                      "height: 0px;"
                                                      "border:0 0 0 0;"
                                                      "visible:false;"
                                                      "}"
                                                      "QScrollBar::down-arrow:vertical{"
                                                      "subcontrol-origin: margin;"
                                                      "height: 0px;"
                                                      "visible:false;"
                                                      "}"
                                                      "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                                      "background: transparent;"
                                                      "border: none;"
                                                      "border-radius: 0px;"
                                                      "}");
        menu_->setStyleSheet("QMenu{"
                             "background-color: rgb(75,75,75);"
                             "}"
                             "QMenu::item:selected{"
                             "background-color: rgba(255,255,255,0.4);"
                             "}");
    }
}

int PersonInfo::dynamicHeight()
{
    int mHeight = height();
    int rowCount = imgListW_->count() / itemCount_ + 1;
    if(rowCount > 1 && (lastRowCount_ != rowCount)){
        mHeight = height() + itemH_ + imgListW_->spacing();
    }
    lastRowCount_ = rowCount;
    if(mHeight < 365)
        mHeight = 365;
    return mHeight;
}

void PersonInfo::addItem(QString s, QImage img)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(s);
    item->setTextAlignment(Qt::AlignCenter);
    item->setIcon(QPixmap::fromImage(img));
    item->setData(Qt::UserRole,img);
    imgListW_->addItem(item);

    if(!itemH_){
        itemH_ = imgListW_->sizeHintForRow(0);
    }

    if(!headImgL_->pixmap()){
        headImgL_->setPixmap(QPixmap::fromImage(img));
        personOrderL_->setText(s);
    }
    personImgCountL_->setText(tr("×%1").arg(imgListW_->count()));
}

void PersonInfo::clearItems()
{
    headImgL_->clear();
    personImgCountL_->clear();
    personOrderL_->clear();
    imgListW_->clear();
}

void PersonInfo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setPen(QColor(194,207,249,51));
    p.drawRoundedRect(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
}

bool PersonInfo::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchW = qobject_cast<QWidget*>(watched);
    if(watchW == imgListW_ && event->type() == QEvent::Resize){
        itemCount_ = (imgListW_->width() -  imgListW_->spacing() - 2 * imgListW_->frameWidth()) / (imgListW_->sizeHintForColumn(0) + imgListW_->spacing());
    }
    return WidgetI::eventFilter(watched,event);
}
