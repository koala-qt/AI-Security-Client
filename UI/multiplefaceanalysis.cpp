#include "multiplefaceanalysis.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QCursor>
#include <QPainter>

#include "pageindicator.h"
#include "nodatatip.h"
#include "waitinglabel.h"
#include "informationdialog.h"

const char * CaptureImgTag = "CaptureImage";
MultipleFaceAnalysis::MultipleFaceAnalysis(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Multiple face analysis"));
    init();
    setUserStyle(userStyle());
    getCameraInfo();
}

void MultipleFaceAnalysis::setUserStyle(int style)
{
    QPalette pal;
    QFont f;
    if (0 == style)
    {
        f = font();
        f.setFamily("PingFang SC Regular");
        setFont(f);

        pal = this->palette();
        pal.setColor(QPalette::Foreground, QColor("#313745"));

        QString commStyle = "font:16px;color:#7E8CB1;font-family:PingFang SC Regular;";
        m_pLabPosition->setStyleSheet(commStyle);
        m_pLabSTime->setStyleSheet(commStyle);
        m_pLabETime->setStyleSheet(commStyle);

        m_pComPosition->setStyleSheet("QComboBox{width:400px;max-width:450px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;}"
                                      "QComboBox::drop-down{"
                                      "subcontrol-position: center right;border-image: url(images/portraitlibrary/icon_arrow.png);width:8px;height:5px;subcontrol-origin: padding;margin-right:5px;"
                                      "}"
                                      "QComboBox QAbstractItemView{"
                                      "background-color:#282D38;"
                                      "selection-color: white;"
                                      "outline: 0px;"
                                      "selection-background-color: #4741F2;"
                                      "}");
        m_pTxtFolderPath->setStyleSheet("width:400px;max-width:450px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;");
        m_pDateSTime->setStyleSheet("width:250px;max-width:250px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;");
        m_pDateETime->setStyleSheet("width:250px;max-width:250px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;");

        // border:1px solid #CECECE;
        m_tableW->setStyleSheet(
                    "QTableView{"
                    "color: #7E8CB1;"
                    "font-size: 12px;"
                    "background-color: transparent;"
                    "selection-background-color: rgba(206,206,206,40);"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgba(206,206,206,20);"
                    "}"
                    "QHeaderView{"
                    "background-color: rgba(206,206,206,20);"
                    "}"
                    "QHeaderView::section{"
                    "color: #7E8CB1;"
                    "background-color: rgba(206,206,206,40);"
                    "}");
        m_tableW->verticalScrollBar()->setStyleSheet(
                                                    "QScrollBar:vertical{"
                                                    "background: transparent;"
                                                    "border-radius: 10px;"
                                                    "border: none;"
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
                                                    "border-radius: 10px;"
                                                    "}");

        m_pageIndicator->setUserStyle();

        m_pBtnSearch->setStyleSheet("QPushButton{"
                                   "border: 0px;min-width:160px;"
                                   "color:white;"
                                   "font-size:12px;width:99px;height:34px;font-family:PingFang SC Regular;"
                                   "border-image:url(images/portraitlibrary/search.png);"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 1px;"
                                   "}");
        m_pBtnUploadFolder->setStyleSheet(m_pBtnSearch->styleSheet());

        m_pDataTip->setUserStyle(style);
    }
}

void MultipleFaceAnalysis::mouseMoveEvent(QMouseEvent *event)
{
    QWidget *wgt = this->childAt(QCursor::pos());
    if (wgt)
    {
        QPushButton *btn = static_cast<QPushButton *>(wgt);
        if (btn && (btn->property("CaptureImgTag") == "true"))
        {
            m_pTipWgt->show();
            m_pTipWgt->move(QCursor::pos());
        }
        else
        {
            m_pTipWgt->hide();
        }
    }
    else
    {
        m_pTipWgt->hide();
    }
    QWidget::mouseMoveEvent(event);
}

void MultipleFaceAnalysis::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    m_pComPosition->clear();
    m_pComPosition->addItem(tr("Unlimited"), "");
    for (auto &info : data)
    {
        m_pComPosition->addItem(info.cameraPos,info.cameraId);
        m_curCameraMapInfo[info.cameraId] = info.cameraPos;
    }
}

void MultipleFaceAnalysis::onBtnUploadFolderClicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,tr("Open Directory"),
                                                           QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty())
    {
        m_pTxtFolderPath->setText(folderPath);
    }
}

void MultipleFaceAnalysis::onBtnSearchClicked()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(m_tableW);
    connect(serviceI, &RestServiceI::sigError, this, [this, label](const QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(m_tableW);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        m_pBtnSearch->setEnabled(true);
        m_pageIndicator->setEnabled(true);
        m_pDataTip->show();
    });
    connect(serviceI, &RestServiceI::sigMNFaceAnalysisResult, this, [&,label](const QVector<RestServiceI::MNFaceAnalysisItem> value){
        label->close();
        delete label;
#if 0
        QVector<RestServiceI::PortraitLibCompItem> value2;
        for (auto item : value)
        {
            value2.append(item);
            value2.append(item);
            value2.append(item);
            value2.append(item);
        }
        slotAddRow(value2);
#else
        slotAddRow(value);
#endif
        m_pBtnSearch->setEnabled(true);
        m_pageIndicator->setEnabled(true);
        if (value.isEmpty())
        {
            m_pDataTip->show();
            return;
        }
    });
    RestServiceI::MNFaceAnalysisArgs args;
    args.strFolderPath = m_pTxtFolderPath->text();
    args.cameraId = m_pComPosition->currentIndex();
    args.startTime = m_pDateSTime->dateTime();
    args.endTime = m_pDateETime->dateTime();
    serviceI->mnFaceAnalysisSearch(args);
    label->show(500);
    m_pBtnSearch->setEnabled(false);
    m_pageIndicator->setEnabled(false);
    m_pDataTip->hide();
    m_tableW->model()->removeRows(0, m_tableW->rowCount());
}

void MultipleFaceAnalysis::init()
{
    QVBoxLayout *mainLay = new QVBoxLayout;

    // top area
    QVBoxLayout *topVLay = new QVBoxLayout;
    mainLay->addLayout(topVLay);

    QHBoxLayout *hlay = new QHBoxLayout;
    topVLay->addLayout(hlay);
    m_pBtnUploadFolder = new QPushButton(tr("Upload folder"));
    m_pBtnUploadFolder->setIcon(QIcon("images/m_n/uploadico.png"));
    connect(m_pBtnUploadFolder, SIGNAL(clicked(bool)),
            this, SLOT(onBtnUploadFolderClicked()));
    hlay->addWidget(m_pBtnUploadFolder);
    m_pTxtFolderPath = new QLineEdit;
    m_pTxtFolderPath->setReadOnly(true);
    hlay->addWidget(m_pTxtFolderPath);
    hlay->addStretch();

    hlay = new QHBoxLayout;
    hlay->setSpacing(10);
    m_pLabPosition = new QLabel(tr("Position"));
    hlay->addWidget(m_pLabPosition);
    m_pComPosition = new QComboBox;
    hlay->addWidget(m_pComPosition);
    m_pLabSTime = new QLabel(tr("Starting time"));
    hlay->addWidget(m_pLabSTime);
    m_pDateSTime = new QDateTimeEdit;
    m_pDateSTime->setDate(QDateTime::currentDateTime().addDays(-1).date());
    hlay->addWidget(m_pDateSTime);
    m_pLabETime = new QLabel(tr("End time"));
    hlay->addWidget(m_pLabETime);
    m_pDateETime = new QDateTimeEdit;
    m_pDateETime->setDate(QDateTime::currentDateTime().addDays(1).date());
    hlay->addWidget(m_pDateETime);
    m_pBtnSearch = new QPushButton(tr("Search"));
    hlay->addWidget(m_pBtnSearch);
    hlay->addStretch();
    connect(m_pBtnSearch, SIGNAL(clicked(bool)),
            this, SLOT(onBtnSearchClicked()));
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay);

    // content area
    m_tableW = new QTableWidget;
    mainLay->addWidget(m_tableW);
    //m_tableW->setIconSize(QSize(112,112));
    m_tableW->setFocusPolicy(Qt::NoFocus);
    m_tableW->setShowGrid(false);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->setEditTriggers(QTableView::NoEditTriggers);
    m_tableW->horizontalHeader()->setHighlightSections(false);
    m_tableW->horizontalHeader()->setDefaultSectionSize(112);
    m_tableW->verticalHeader()->setDefaultSectionSize(112);
    m_tableW->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableW->setColumnCount(6);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("Upload Photo") << tr("Capture1") << tr("Capture2") << tr("Capture3") << tr("Capture4") << tr("Capture5")); // Big type and small type.  << tr("Type"))
    m_tableW->setColumnWidth(UploadImgCol, 180);
    m_tableW->horizontalHeader()->setSortIndicatorShown(false);
    connect(m_tableW->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSectionClicked(int)));
    m_pageIndicator = new PageIndicator;
    m_pageIndicator->setPageInfo(0,0);
    m_pageIndicator->hide();
    hlay = new QHBoxLayout;
    hlay->addWidget(m_pageIndicator);
    hlay->setAlignment(Qt::AlignCenter);
    mainLay->addLayout(hlay);
    mainLay->setContentsMargins(30,20,10,10);
    setLayout(mainLay);

    m_pDataTip = new NoDataTip(m_tableW);
    m_pTipWgt = new QWidget;
    m_pTipWgt->setWindowFlags(Qt::FramelessWindowHint);
    m_pTipWgt->setStyleSheet("background:rgba(2,4,18,0.78);");
    m_pTipWgt->hide();
}

void MultipleFaceAnalysis::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void MultipleFaceAnalysis::slotAddRow(QVector<RestServiceI::MNFaceAnalysisItem> info)
{
    if (info.isEmpty())
    {
        m_pDataTip->show();
    }
//    for (int j = 0; j < 4; ++j) // test scroll
//    {
    for (const RestServiceI::MNFaceAnalysisItem &itemData : info)
    {
        m_tableW->insertRow(m_tableW->rowCount());
        m_tableW->setRowHeight(m_tableW->rowCount(), 120);
        QTableWidgetItem *item = new QTableWidgetItem;
#if 0
        item->setIcon(QPixmap::fromImage(m_faceImg));
        item->setTextAlignment(Qt::AlignCenter);
        //item->setData(Qt::UserRole,itemData.faceImg);
        m_tableW->setItem(m_tableW->rowCount() - 1,ImageCol,item);
#else
        QLabel *tempLab = new QLabel;
        tempLab->setPixmap(QPixmap::fromImage(itemData.uploadImg).scaled(100, 100));
        tempLab->setAlignment(Qt::AlignCenter);
        m_tableW->setCellWidget(m_tableW->rowCount() - 1, UploadImgCol, tempLab);
#endif
        int columnIndex = 1;
        //QLabel *captureLab;
        QPushButton *captureLab = Q_NULLPTR;
        for (const RestServiceI::MNCaptureItem &captureitem : itemData.captureItems)
        {
            //aptureLab = new QLabel;
            //captureLab->setPixmap(QPixmap::fromImage(captureitem.captureImg).scaled(100, 100));
            captureLab = new QPushButton;
            captureLab->setStyleSheet("background-color:transparent;border:0px;");
//            connect(captureLab, &QPushButton::clicked, this, [this]{
//                m_pTipWgt->move(QCursor::pos());
//                m_pTipWgt;
//            });
            captureLab->setProperty(CaptureImgTag, "true");
            captureLab->setIcon(QPixmap::fromImage(captureitem.captureImg).scaled(100, 100));
            captureLab->setIconSize(QSize(100, 100));
            //captureLab->setAlignment(Qt::AlignCenter);
            m_tableW->setCellWidget(m_tableW->rowCount() - 1, columnIndex, captureLab);
            columnIndex++;
        }
        //auto captureItems = itemData.captureItems;

    }
}
