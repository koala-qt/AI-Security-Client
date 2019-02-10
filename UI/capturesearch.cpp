#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QLabel>
#include <QScrollBar>
#include <QMenu>
#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QTreeWidget>
#include <QMessageBox>
#include <QSettings>
#include <QFontMetrics>
#include "facesearch.h"
#include "pageindicator.h"
#include "waitinglabel.h"
#include "service/restservice.h"
#include "capturesearch.h"
#include "sceneimagedialog.h"
#include "components/SelectImage/selectimage.h"
#include "informationdialog.h"
#include "nodatatip.h"

#pragma execution_character_set("utf-8")
CaptureSearch::CaptureSearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Capture face history"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(15);
    m_cameraL = new QLabel(tr("位置"));
    cameraCombox_ = new QComboBox;
    cameraCombox_->setIconSize(QSize(1,30));
    cameraCombox_->setFocusPolicy(Qt::NoFocus);
    cameraCombox_->setFixedHeight(40);
    m_startTimeL = new QLabel(tr("开始时间"));
    m_endTimeL = new QLabel(tr("结束时间"));
    m_startTimeEdit = new QDateTimeEdit;
    m_startTimeEdit->setMinimumSize(160,40);
    m_endTimeEdit = new QDateTimeEdit;
    m_endTimeEdit->setMinimumSize(160,40);
    m_searchBtn = new QPushButton(tr("查询"));
    m_searchBtn->setFixedSize(90,40);
    hlay->addWidget(m_cameraL);
    hlay->addWidget(cameraCombox_);
    hlay->addWidget(m_startTimeL);
    hlay->addWidget(m_startTimeEdit);
    hlay->addWidget(m_endTimeL);
    hlay->addWidget(m_endTimeEdit);
    hlay->addWidget(m_searchBtn);
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay);

    m_listW = new QListWidget;
    mainLay->addWidget(m_listW);

    m_pageIndicator = new PageIndicator;
    m_pageIndicator->setPageInfo(0,0);
    mainLay->addWidget(m_pageIndicator);
    setLayout(mainLay);

    menu_ = new QMenu(this);
    menu_->addAction(tr("Search using the image"),[&]{
        FaceSearch *faceDialog = new FaceSearch(widgetManger());
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(112,110,119));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(widgetManger()->currentStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(m_listW->currentItem()->data(Qt::UserRole + 1).value<QImage>());
//        faceDialog->setOid(m_listW->currentItem()->data(Qt::UserRole + 2).toString());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    menu_->addAction(tr("Scene analysis"),[&]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[&,label](const QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(widgetManger()->currentStyle());
            infoDialog.showMessage(str);
            infoDialog.exec();
            menu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            slotOnSceneInfo(sinfo);
            menu_->setEnabled(true);
        });
        serviceI->getSceneInfo(m_listW->currentItem()->data(Qt::UserRole + 2).toString());
        startWorker(worker);
        label->show(500);
        menu_->setEnabled(false);
    });
    menu_->addAction(tr("Save face image"),[this]{
        QString personId = m_listW->currentItem()->data(Qt::UserRole + 2).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!m_listW->currentItem()->data(Qt::UserRole + 1).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(widgetManger()->currentStyle());
            infoDialog.showMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    m_startTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    m_endTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    m_startTimeEdit->setDate(QDateTime::currentDateTime().addDays(-1).date());
    m_endTimeEdit->setDate(QDateTime::currentDateTime().addDays(1).date());
    m_listW->setViewMode(QListWidget::IconMode);
    m_listW->setFlow(QListWidget::LeftToRight);
    m_listW->setEditTriggers(QListWidget::NoEditTriggers);
    m_listW->setResizeMode(QListWidget::Adjust);
    m_listW->setMovement(QListWidget::Static);
    m_listW->setContextMenuPolicy(Qt::CustomContextMenu);
    m_listW->horizontalScrollBar()->resize(0,0);
    m_listW->verticalScrollBar()->resize(0,0);
    m_listW->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_listW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(m_searchBtn,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(m_pageIndicator,SIGNAL(sigPageClicked(int)),this,SLOT(slotSearchSnapInfo(int)));
    connect(m_listW,&QListWidget::customContextMenuRequested,this,[&](QPoint p){
        if(!m_listW->itemAt(p))return;
        menu_->move(QCursor::pos());
        menu_->show();
    });
    noDataW_ = new NoDataTip(m_listW);

    QSettings config("config.ini",QSettings::IniFormat);
    dataRows_ = config.value("App/CaptureRows").toInt();
    dataCols_ = config.value("App/CaptureCols").toInt();
    getCameraInfo();
}

void CaptureSearch::setUserStyle(WidgetManagerI::SkinStyle style)
{
    QFont f;
    QPalette pal;
    if(style == WidgetManagerI::Danyahei){
        f = font();
        f.setFamily("Arial");
        setFont(f);
        f = m_cameraL->font();
        f.setPixelSize(18);
        m_cameraL->setFont(f);
        m_startTimeL->setFont(f);
        m_endTimeL->setFont(f);

        f = m_listW->font();
        f.setPixelSize(11);
        m_listW->setFont(f);

        pal = m_cameraL->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        m_cameraL->setPalette(pal);
        m_startTimeL->setPalette(pal);
        m_endTimeL->setPalette(pal);


        cameraCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: transparent;"
                    "border: 1px solid #CECECE;"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "selection-color: white;"
                    "outline: 0px;"
                    "selection-background-color: #CECECE;"
                    "}"
                    "QComboBox::drop-down{"
                    "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                    "}"
                    "QScrollBar:vertical{"
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

        m_startTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color:#CECECE;"
            "border:1px solid #CECECE;"
            "border-radius:6px;"
            "background-color: transparent;"
                                       "}");

        m_endTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color:#CECECE;"
            "border:1px solid #CECECE;"
            "border-radius:4px;"
            "background-color: transparent;"
                                       "}");

        m_searchBtn->setStyleSheet("QPushButton{"
                                  "background-color: #B4A06C;"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "font-size:18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: rgba(255,0,0,100);"
                                  "}");

        pal = m_listW->palette();
        pal.setColor(QPalette::Base,Qt::transparent);
        pal.setColor(QPalette::Text,Qt::white);
        m_listW->setPalette(pal);
        m_listW->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                                    "background: transparent;"
                                                    "border: none;"
                                                    "}"
                                                    "QScrollBar::handle:vertical{"
                                                    "background: transparent;"
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
                                                    "background: none;"
                                                    "}");
        m_listW->horizontalScrollBar()->setStyleSheet(
                                                    "QScrollBar:horizontal{"
                                                    "background: transparent;"
                                                    "border: none;"
                                                    "}"
                                                    "QScrollBar::handle:horizontal{"
                                                    "background: transparent;"
                                                    "}"
                                                    "QScrollBar::add-line:horizontal{"
                                                    "background: transparent;"
                                                    "height: 0px;"
                                                    "}"
                                                    "QScrollBar::sub-line:horizontal{"
                                                    "background: transparent;"
                                                    "height: 0px;"
                                                    "}"
                                                    "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{"
                                                    "background: none;"
                                                    "}");
        menu_->setStyleSheet("QMenu{"
                                 "background-color: rgb(75,75,75);"
                                 "}"
                                 "QMenu::item:selected{"
                                 "background-color: rgba(255,255,255,0.4);"
                                 "}");

        m_pageIndicator->setUserStyle();
        noDataW_->setUserStyle(style);
    }
}

void CaptureSearch::resizeEvent(QResizeEvent *event)
{
    int w = (m_listW->width() - style()->pixelMetric(QStyle::PM_ScrollBarSliderMin) - m_listW->frameWidth() * 2 - (dataCols_ + 1) * m_listW->spacing()) / dataCols_;
    int h = (m_listW->height() - style()->pixelMetric(QStyle::PM_ScrollBarSliderMin)  - m_listW->frameWidth() * 2 - (dataRows_ + 1) * m_listW->spacing()) / dataRows_;
    itemSize_.setWidth(w);
    itemSize_.setHeight(h);

    QFontMetrics fm = m_listW->fontMetrics();
    iconSize_.setWidth(itemSize_.width() - 4);
    iconSize_.setHeight(itemSize_.height() - fm.height() * 2 - 5);
    m_listW->setIconSize(iconSize_);
    for(int i = 0; i < m_listW->count(); i++){
        QListWidgetItem *item = m_listW->item(i);
        item->setSizeHint(itemSize_);
        item->setIcon(QPixmap::fromImage(item->data(Qt::UserRole + 1).value<QImage>().scaled(m_listW->iconSize())));
    }

    return WidgetI::resizeEvent(event);
}

bool CaptureSearch::event(QEvent *event)
{
    if(event->type() == QEvent::Show  && m_searchBtn->isEnabled()){
        m_endTimeEdit->setDateTime(QDateTime::currentDateTime());
        slotSearchBtnClicked();
        return true;
    }
    return WidgetI::event(event);
}

void CaptureSearch::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void CaptureSearch::slotOnSceneInfo(RestServiceI::SceneInfo sinfo)
{
#if 1
    SceneImageDialog dialog;
    dialog.setUserStyle(widgetManger()->currentStyle());
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    dialog.setSceneInfo(sinfo);
    dialog.setRectLinePen(Qt::yellow);
    connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
        if(!images.count()){
            return;
        }
        FaceSearch *faceDialog = new FaceSearch(widgetManger());
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(112,110,119));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(widgetManger()->currentStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(images.first());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    dialog.exec();
#else
    QDialog dialog;
    QLabel *label = new QLabel;
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(label);
    dialog.setLayout(lay);
    dialog.setModal(true);

    label->setScaledContents(true);
    label->setPixmap(QPixmap::fromImage(img));
    dialog.setFixedSize(960,540);
    dialog.exec();
#endif
}

void CaptureSearch::slotSearchBtnClicked()
{
    curCameraId_ = cameraCombox_->currentData().toString();
    curStartTime_ = m_startTimeEdit->dateTime();
    curEndTime_ = m_endTimeEdit->dateTime();
    needUpdatePageInfo_ = true;
    slotSearchSnapInfo(1);
}

void CaptureSearch::slotSearchSnapInfo(int page)
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(widgetManger()->currentStyle());
        infoDialog.showMessage(str);
        infoDialog.exec();
        m_searchBtn->setEnabled(true);
        m_pageIndicator->setEnabled(true);
        noDataW_->show();
    });
    connect(serviceI,&RestServiceI::sigCaptureSearch,this,[this,label](const RestServiceI::CaptureSearchReturnData value){
        label->close();
        delete label;
        m_listW->clear();
        slotOnSearch(value);
        m_searchBtn->setEnabled(true);
        m_pageIndicator->setEnabled(true);
    });
    RestServiceI::CaptureSearchArgs args;
    args.page = page;
    args.pageCount = dataRows_ * dataCols_;
    args.position = curCameraId_;
    args.start = curStartTime_;
    args.end = curEndTime_;
    serviceI->captureSearch(args);
    startWorker(worker);
    label->show(500);
    m_searchBtn->setEnabled(false);
    m_pageIndicator->setEnabled(false);
    noDataW_->hide();
    m_listW->clear();
}

void CaptureSearch::slotOnSearch(RestServiceI::CaptureSearchReturnData data)
{
    m_pageIndicator->adjustRow();
    if(needUpdatePageInfo_){
        m_pageIndicator->setPageInfo(data.totalPage,data.totalCount);
        needUpdatePageInfo_ = false;
    }
    if(data.data.isEmpty()){
        noDataW_->show();
    }
    QFontMetrics fs(m_listW->font());
    int textwidth = itemSize_.width() / (fs.height() >> 1);
    for (RestServiceI::DataRectureItem &info : data.data) {
        QString cameraPosStr = curCameraMapInfo_.value(info.cameraId);
        if(fs.width(cameraPosStr) >  itemSize_.width()){
            cameraPosStr = cameraPosStr.left(textwidth);
            cameraPosStr.remove(cameraPosStr.count() - 4, 3);
            cameraPosStr.append("...");
        }
        QListWidgetItem *item = new QListWidgetItem(cameraPosStr + '\n' +
                                                    info.time.toString("yyyy-MM-dd HH:mm:ss"));
        item->setSizeHint(itemSize_);
        item->setIcon(QPixmap::fromImage(info.img));
        item->setData(Qt::UserRole+1,info.img);
        item->setData(Qt::UserRole+2,info.sceneId);
        item->setToolTip(curCameraMapInfo_.value(info.cameraId));
        item->setTextAlignment(Qt::AlignHCenter);
        m_listW->addItem(item);
    }
}

void CaptureSearch::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    QPixmap pix(cameraCombox_->iconSize());
    pix.fill(Qt::transparent);
    cameraCombox_->clear();
    cameraCombox_->addItem(pix,tr("不限"),"");
    for (auto &info : data) {
        cameraCombox_->addItem(pix,info.cameraPos,info.cameraId);
        curCameraMapInfo_[info.cameraId] = info.cameraPos;
    }
}
