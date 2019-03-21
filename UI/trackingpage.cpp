#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QApplication>
#include <QTimer>
#include "trackingpage.h"
#include "trackingwebview.h"
#include "waitinglabel.h"
#include "informationdialog.h"
#include "videoplayer.h"

TrackingPage::TrackingPage( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Trajectory"));
    imgBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("Search"));
    startTimeL_ = new QLabel(tr("Starting Time"));
    endTimeL_ = new QLabel(tr("Ending Time"));
    threshL_ = new QLabel(tr("Threshold"));
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    threshSpin_ = new QSpinBox;
    dataView_ = new TrackingWebView;

    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *topHLay = new QHBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    topHLay->addWidget(imgBtn_);
    gridLay->addWidget(threshL_,0,0,1,1);
    gridLay->addWidget(threshSpin_,0,1,1,1);
    gridLay->addWidget(startTimeL_,1,0,1,1);
    gridLay->addWidget(startTimeEdit_,1,1,1,1);
    gridLay->addWidget(endTimeL_,1,2,1,1);
    gridLay->addWidget(endTimeEdit_,1,3,1,1);
    gridLay->addWidget(searchBtn_,1,4,1,1);
    gridLay->setSpacing(20);
    gridLay->setMargin(0);
    topHLay->addLayout(gridLay);
    topHLay->setAlignment(Qt::AlignLeft);
    topHLay->setSpacing(20);
    topHLay->setMargin(0);
    mainLay->addLayout(topHLay);
    mainLay->addWidget(dataView_);
    mainLay->setMargin(40);
    mainLay->setSpacing(20);
    setLayout(mainLay);

    threshSpin_->setRange(0,100);
    threshSpin_->setSuffix("%");
    threshSpin_->setFixedSize(200,34);
    startTimeEdit_->setFixedSize(200,34);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    threshSpin_->setValue(53);
    endTimeEdit_->setFixedSize(200,34);
    QPixmap defaultPersonBackPix("images/person-face-back.png");
    imgBtn_->setFixedSize(defaultPersonBackPix.size());
    imgBtn_->setIconSize(imgBtn_->size());
    imgBtn_->setIcon(defaultPersonBackPix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("default-pix",defaultPersonBackPix);
    QCursor imgBtnCoursor = imgBtn_->cursor();
    imgBtnCoursor.setShape(Qt::PointingHandCursor);
    imgBtn_->setCursor(imgBtnCoursor);
    imgBtn_->setFocusPolicy(Qt::NoFocus);

    connect(imgBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImgBtnClicked()));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));

    QSettings configSetting("config.ini",QSettings::IniFormat);
    hostname_ = configSetting.value("Http/Javahost").toString();

    connect(dataView_,SIGNAL(sigCameraClicked(QString)),this,SLOT(slotOnCameraClicked(QString)));
    connect(dataView_,SIGNAL(sigWebError(QString)),this,SLOT(slotOnWebError(QString)));
    setUserStyle(userStyle());
    getCameraInfo();
    queryPersonTypes();
}

void TrackingPage::setUserStyle(int s)
{
    QPalette pal;
    if(0 == s){
        imgBtn_->setStyleSheet("QPushButton{"
                               "background-color: transparent;"
                               "}");
        threshL_->setStyleSheet("QLabel{"
                                "color: rgba(255,255,255,191);"
                                "font-size: 14px;"
                                "}");
        startTimeL_->setStyleSheet("QLabel{"
                                   "color: rgba(255,255,255,191);"
                                   "font-size: 14px;"
                                   "}");
        endTimeL_->setStyleSheet("QLabel{"
                                 "color: rgba(255,255,255,191);"
                                 "font-size: 14px;"
                                 "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "color: rgba(255,255,255,191);"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "border-radius: 4px;"
                                    "padding-left: 10px;"
                                    "font-size: 14px;"
                                    "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                      "color: rgba(255,255,255,191);"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "border-radius: 4px;"
                                      "padding-left: 10px;"
                                      "font-size: 14px;"
                                      "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "font-size: 18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        threshSpin_->setStyleSheet("QSpinBox{"
                                   "color: rgba(255,255,255,191);"
                                   "background-color: rgba(255,255,255,0.1);"
                                   "border-radius: 4px;"
                                   "padding-left: 10px;"
                                   "font-size: 14px;"
                                   "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "width: 99px;"
                                  "height: 34px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
    }
}

void TrackingPage::setImgageOid(QImage img, QString oid)
{
    curOid_ = oid;
    QPixmap pix = QPixmap::fromImage(img.scaled(imgBtn_->iconSize()));
    imgBtn_->setIcon(pix);
    imgBtn_->setProperty("pixmap",QPixmap::fromImage(img));
    m_faceImg = img;
}

void TrackingPage::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void TrackingPage::slotImgBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("add image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    QPixmap pix(filePath);
    if(pix.isNull()){
        imgBtn_->setIcon(imgBtn_->property("default-pix").value<QPixmap>());
        imgBtn_->setProperty("pixmap",QPixmap());
        return;
    }
    imgBtn_->setIcon(pix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("pixmap",pix);
    curOid_.clear();
    m_faceImg = pix.toImage();
}

void TrackingPage::slotSearchBtnClicked()
{
    waitingL_ = new WaitingLabel(dataView_);
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    RestServiceI::FaceTrackingArgs args;
    args.oid = curOid_;
    args.faceImg = imgBtn_->property("pixmap").value<QPixmap>().toImage();
    args.startT = startTimeEdit_->dateTime();
    args.endT = endTimeEdit_->dateTime();
    args.thresh = threshSpin_->value() / qreal(100);
    connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
        waitingL_->close();
        delete waitingL_;
        waitingL_ = nullptr;
        dataView_->updateTracking(QVector<TrackingWebView::TrackingPoint>());
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        searchBtn_->setEnabled(true);
    });
#if 0
    connect(serviceI,&RestServiceI::sigTracking,this,[this,label](const QVector<SearchFace> data){
        label->close();
        delete label;
        slotTracking(data);
        searchBtn_->setEnabled(true);
    });
#else
    connect(serviceI,&RestServiceI::sigTrackingNew,this,[this](const QVector<RestServiceI::TrackingReturnData> data){
        waitingL_->close();
        delete waitingL_;
        waitingL_ = nullptr;
        slotTrackingNew(data);
        searchBtn_->setEnabled(true);
    });
#endif
    serviceI->faceTracking(args);
    waitingL_->show(500);

    // 3.21 add 1:n
    portraitSearch();
    searchBtn_->setEnabled(false);
    //dataView_->startWaiting();
}

void TrackingPage::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    for(RestServiceI::CameraInfo &info : data){
        curCameraMap_[info.cameraId] = info;
    }
}

void TrackingPage::slotTrackingNew(QVector<RestServiceI::TrackingReturnData> data)
{
    QVector<TrackingWebView::TrackingPoint> trackingVec;
    std::transform(data.begin(),data.end(),std::back_inserter(trackingVec),[this](const RestServiceI::TrackingReturnData &value){
        TrackingWebView::TrackingPoint pointData;
        pointData.cameraId = value.cameraId;
        pointData.name = curCameraMap_.value(value.cameraId).cameraPos;
        pointData.grabTime = value.timeIn.toString("yyyy-MM-dd HH:mm:ss");
        int holdTime = (value.timeOut.toMSecsSinceEpoch() - value.timeIn.toMSecsSinceEpoch())/1000;
        pointData.holdTime.setNum(holdTime < 1 ? 1 : holdTime);
//        pointData.personImgUr = hostname_ + "graph/node/picture/" + value.objId;
        pointData.personImgUr = hostname_ + "api/v2/cmcc/monitor/alarm/query/sms-face/" + value.objId;
        pointData.lat = value.lat;
        pointData.lng = value.lng;
        pointData.sceneId = value.sceneId;
//        qDebug() << pointData.cameraId << pointData.name << pointData.grabTime << pointData.holdTime << pointData.personImgUr << pointData.lat << pointData.lng;
        return pointData;
    });
    dataView_->updateTracking(trackingVec);
}

void TrackingPage::slotTracking(QVector<SearchFace> data)
{
    QVector<TrackingWebView::TrackingPoint> trackingVec;
    std::transform(data.begin(),data.end(),std::back_inserter(trackingVec),[this](const SearchFace &value){
        TrackingWebView::TrackingPoint pointData;
        pointData.name = QString::fromStdString(value.camera_pos);
        pointData.grabTime = QDateTime::fromMSecsSinceEpoch(value.time).toString("yyyy-MM-dd HH:mm:ss");
        pointData.personImgUr = hostname_ + "graph/node/picture/" + QString::fromStdString(value.oid);
        return pointData;
    });
    dataView_->updateTracking(trackingVec);
}

void TrackingPage::slotOnCameraClicked(QString cameraId)
{
    VideoPlayer *player = new VideoPlayer(this);
    player->setAttribute(Qt::WA_DeleteOnClose);
    player->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    player->setWindowModality(Qt::ApplicationModal);
    player->setMinimumSize(960,540);
    player->startPlay(curCameraMap_.value(cameraId).rtsp,"fmg_decoder");
    player->show();
}

void TrackingPage::slotOnWebError(QString str)
{
    InformationDialog infoDialog(this);
    infoDialog.setUserStyle(userStyle());
    infoDialog.setMessage(str);
    infoDialog.exec();
}

void TrackingPage::queryPersonTypes()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI, &RestServiceI::sigError, this, [this](const QString str){
    });
    connect(serviceI, &RestServiceI::sigPersonTypesResult, this, [&](const QVector<RestServiceI::PersonType> value){
        auto iter = value.begin();
        QPushButton *m_btnFaceType = Q_NULLPTR;
        for (iter; iter != value.end(); ++iter)
        {
            m_strPersonTypes += iter->strTypeNo + ",";
        }
    });
    serviceI->queryPersonTypes();
}

void TrackingPage::portraitSearch()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI, &RestServiceI::sigError, this, [this](const QString str){
    });
    connect(serviceI, &RestServiceI::sigPortraitLibCompResult, this, [&](const QVector<RestServiceI::PortraitLibCompItem> value){
        dataView_->updatePersonInfo(value);
    });
    RestServiceI::PortraitLibCompArgs args;
    args.image = m_faceImg;
    args.bRequireBase64 = true;

    args.libType = m_strPersonTypes;
    //args.similarity = 0.3f;
    args.similarity = threshSpin_->value() / qreal(100);
    args.limit = 1;
    args.sourceType = tr("2");
    args.nPersonId = 0;
    serviceI->portraitLibCompSearch(args);
}
