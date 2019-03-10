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
#include <QGroupBox>
#include <QRadioButton>
#include <QCursor>
#include <QPainter>

#include "pageindicator.h"
#include "nodatatip.h"
#include "waitinglabel.h"
#include "informationdialog.h"

#pragma execution_character_set("utf-8")
const char * CaptureImgTag = "CaptureImage";
const char* MNFaceTypeCheckedTag = "FaceTypeChecked";
MultipleFaceAnalysis::MultipleFaceAnalysis(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Multiple Search"));
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

        QString commStyle = "font-size:16px;color:#7E8CB1;font-family:PingFang SC Regular;";
        m_pLabDbTitle->setStyleSheet(commStyle);
        m_pLabID->setStyleSheet(commStyle);
        m_txtID->setStyleSheet("width:150px;max-width:250px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;");
        m_pLabName->setStyleSheet("font:16px;color:#7E8CB1;font-family:PingFang SC Regular;width:40px;");
        m_pTxtName->setStyleSheet(m_txtID->styleSheet());
        m_pLabSimilary->setStyleSheet(commStyle);
        m_pTxtSimilary->setStyleSheet("width:180px;max-width:250px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-size:16px;font-family:PingFang SC Regular;");
        m_pFaceTypesWgt->setStyleSheet(".QWidget{border-image:url(images/portraitlibrary/barbg.png);background-color:transparent;min-width:400px;height:45px;max-height:45px;}");
        m_pDbType->setStyleSheet(".QComboBox{width:180px;max-width:180px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;font-size:16px;}"
                                 "QComboBox::drop-down{"
                                 "subcontrol-position: center right;border-image: url(images/portraitlibrary/icon_arrow.png);width:8px;height:10px;subcontrol-origin: padding;margin-right:5px;"
                                 "}"
                                 "QComboBox QAbstractItemView{"
                                 "background-color:#282D38;"
                                 "selection-color: white;"
                                 "outline: 0px;"
                                 "selection-background-color: #4741F2;height:50px;"
                                 "}"
                                 "QComboBox QAbstractItemView::item {height:25px; min-height: 25px; }");
#if 0
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
        m_pDateSTime->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "font-family:PingFang SC Regular;"
                                    "width:250px;max-width:250px;height:34px;"
                                    "color: rgb(126,140,177);"
//                                    "box-shadow:1px 1px 0px rgba(77,86,107,1);"
                                    "border-radius:6px;"
                                    "background-color: rgb(40,45,56);}");
        m_pDateETime->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "font-family:PingFang SC Regular;"
                                    "width:250px;max-width:250px;height:34px;"
                                    "color: rgb(126,140,177);"
//                                    "box-shadow:1px 1px 0px rgba(77,86,107,1);"
                                    "border-radius:6px;"
                                    "background-color: rgb(40,45,56);}");
#endif
        m_pTxtFolderPath->setStyleSheet("width:450px;max-width:450px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;");


        // border:1px solid #CECECE;
        m_tableW->setStyleSheet(
                    "QTableView{"
                    "color: #7E8CB1;"
                    "font-size: 12px;"
                    "background-color: #383F4F;"
                    "border:0px;"
                    //"selection-background-color: rgba(206,206,206,40);"
                    "}"
                    "QTableWidget::item"
                    "{"
                    "color: #7E8CB1;"
                    "}"
                    "QTableWidget::item::selected"
                    "{"
                    "background-color: rgba(206,206,206,40);"
                    "color: #7E8CB1;"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgba(206,206,206,20);"
                    "}");
        m_tableW->horizontalHeader()->setStyleSheet("QHeaderView{"
                                                    "background-color: #41495C;"
                                                    "}"
                                                    "QHeaderView::section{"
                                                    "background-color: transparent;"
                                                    "color:#7E8CB1;font-size:14px;font:bold;"
                                                    "}");
        m_tableW->verticalScrollBar()->setStyleSheet(
                                                    "QScrollBar:vertical{"
                                                    "background: #383F4F;"
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
                                   "border: 0px;"
                                   "color:white;"
                                   "font-size:12px;width:156px;height:34px;font-family:Regular;"
                                   "border-image:url(images/portraitlibrary/search.png);"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 1px;"
                                   "}");
        m_pBtnUploadFolder->setStyleSheet("QPushButton{"
                                          "border: 0px;"
                                          "color:white;"
                                          "font-size:12px;width:120px;height:34px;font-family:Regular;"
                                          "border-image:url(images/portraitlibrary/search.png);"
                                          "}"
                                          "QPushButton:pressed{"
                                          "padding: 1px;"
                                          "}");

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

bool MultipleFaceAnalysis::event(QEvent *event)
{
    if (m_bFirstLoading && (event->type() == QEvent::Show))
    {
        queryPersonTypes();
        m_bFirstLoading = false;
        return true;
    }
    return WidgetI::event(event);
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
    args.sourceType = (0 == m_pDbType->currentIndex()) ? tr("1") : tr("2");
    args.strFolderPath = m_pTxtFolderPath->text();
    args.bRequireBase64 = true;
#if 0
    args.cameraId = m_pComPosition->currentIndex();
    args.startTime = m_pDateSTime->dateTime();
    args.endTime = m_pDateETime->dateTime();
#endif
    QString strTypes;
    int index = 0;
    for (auto btnTypeItem : m_lstFaceLibTypes)
    {
        if (btnTypeItem->property(MNFaceTypeCheckedTag).toBool())
        {
            if (!strTypes.isEmpty())
            {
                strTypes.append(",");
            }
            strTypes.append(m_vecPersonTypes[index].strTypeNo);
        }
        index++;
    }
    args.libType = strTypes;
    //m_strBigPersonType = m_faceLibBar->tabText(m_faceLibBar->currentIndex());
    args.similarity = m_pTxtSimilary->text().toDouble();
    args.limit = 5;
#if 0
    if (!m_txtID->text().isEmpty())
    {
        args.nPersonId = m_txtID->text().toInt();
    }
    else
    {
        args.nPersonId = 0;
    }
    args.strPersonName = m_pTxtName->text();
#endif
    serviceI->mnFaceAnalysisSearch(args);
    label->show(500);
    m_pBtnSearch->setEnabled(false);
    m_pageIndicator->setEnabled(false);
    m_pDataTip->hide();
    m_tableW->model()->removeRows(0, m_tableW->rowCount());
}

void MultipleFaceAnalysis::onBtnOperationClicked()
{
    updateBtnPersonTypeStyle(m_pBtnOperation);
    if (!m_pBtnOperation->property(MNFaceTypeCheckedTag).toBool())
    {
        updatePersonTypesState();
        m_pBtnOperation->setText(tr("Select all"));
    }
    else
    {
        updatePersonTypesState();
        m_pBtnOperation->setText(tr("Unselect all"));
    }
}

void MultipleFaceAnalysis::init()
{
    libGroupBox_ = new QGroupBox;
    markRadiuBtn_ = new QRadioButton(tr("mark"));
    registRadiuBtn_ = new QRadioButton(tr("regist"));
    markRadiuBtn_->setChecked(true);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(markRadiuBtn_);
    hlay->addWidget(registRadiuBtn_);
    libGroupBox_->setLayout(hlay);

    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->setSpacing(20);

    // top area
    QVBoxLayout *topVLay = new QVBoxLayout;
    mainLay->addLayout(topVLay);

    hlay = new QHBoxLayout;
    topVLay->addLayout(hlay);
    m_pBtnUploadFolder = new QPushButton(tr("Upload folder"));
    m_pBtnUploadFolder->setIcon(QIcon("images/m_n/uploadico.png"));
    connect(m_pBtnUploadFolder, SIGNAL(clicked(bool)),
            this, SLOT(onBtnUploadFolderClicked()));
    hlay->addWidget(m_pBtnUploadFolder);
    m_pTxtFolderPath = new QLineEdit;
    m_pTxtFolderPath->setReadOnly(true);
    hlay->addWidget(m_pTxtFolderPath);
    //hlay->addWidget(libGroupBox_);
    hlay->addStretch();


    //topVLay->addStretch();

    QHBoxLayout *bottomHlay = new QHBoxLayout;
    bottomHlay->setSpacing(15);
    topVLay->addLayout(bottomHlay);
    m_pLabID = new QLabel(tr("ID"));
    //bottomHlay->addWidget(m_pLabID);
    m_txtID = new QLineEdit;
    m_txtID->setValidator(new QIntValidator(0, 500000, this));
    //bottomHlay->addWidget(m_txtID);
    m_pLabName = new QLabel(tr("Name"));
    //bottomHlay->addWidget(m_pLabName);
    m_pTxtName = new QLineEdit;
    m_pTxtName->setMaxLength(50);
    //bottomHlay->addWidget(m_pTxtName);
    // 3.1 add
#if 0
    m_pLabLimit = new QLabel(tr("Top"));
    bottomHlay->addWidget(m_pLabLimit);
    m_pLimitCombo = new QComboBox;
    bottomHlay->addWidget(m_pLimitCombo);
    m_pLimitCombo->addItem(tr("10"));
    m_pLimitCombo->addItem(tr("20"));
    m_pLimitCombo->addItem(tr("30"));
    m_pLimitCombo->addItem(tr("50"));
    m_pLimitCombo->addItem(tr("100"));
#endif
    m_pLabSimilary = new QLabel(tr("Similarity"));
    bottomHlay->addWidget(m_pLabSimilary);
    m_pTxtSimilary = new QLineEdit(tr("0.3"));
    m_pTxtSimilary->setValidator(new QDoubleValidator(1, 0.1, 2, this));
    bottomHlay->addWidget(m_pTxtSimilary);
    m_pLabDbTitle = new QLabel(tr("Database"));
    bottomHlay->addWidget(m_pLabDbTitle);
    m_pDbType = new QComboBox;
    bottomHlay->addWidget(m_pDbType);
    m_pDbType->addItem(tr("mark"));
    m_pDbType->addItem(tr("regist"));
    bottomHlay->addStretch();
    bottomHlay->setAlignment(Qt::AlignLeft);

    m_pFaceTypesWgt = new QWidget;
    m_pFaceTypesHLay = new QHBoxLayout;
    m_pFaceTypesHLay->setSpacing(5);
    m_pFaceTypesHLay->setMargin(2);
    m_pFaceTypesWgt->setLayout(m_pFaceTypesHLay);
    QHBoxLayout *faceTypeHLay = new QHBoxLayout;
    faceTypeHLay->addWidget(m_pFaceTypesWgt);
    //faceTypeHLay->addStretch();
    //topVLay->addWidget(m_pFaceTypesWgt);
    m_pBtnSearch = new QPushButton(tr("Search"));
    faceTypeHLay->addWidget(m_pBtnSearch);
    faceTypeHLay->addStretch();
    connect(m_pBtnSearch, SIGNAL(clicked(bool)),
            this, SLOT(onBtnSearchClicked()));
    topVLay->addLayout(faceTypeHLay);

#if 0
    // old version.
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
#endif

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
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("Upload Photo") << tr("Comparison result 1") << tr("Comparison result 2") << tr("Comparison result 3") << tr("Comparison result 4") << tr("Comparison result 5"));
    m_tableW->setColumnWidth(UploadImgCol, 180);
    m_tableW->horizontalHeader()->setSortIndicatorShown(false);
    m_tableW->horizontalHeader()->setMinimumHeight(60);
    //connect(m_tableW->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSectionClicked(int)));
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
#if 0
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
#endif
}

void MultipleFaceAnalysis::queryPersonTypes()
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
    });
    connect(serviceI, &RestServiceI::sigPersonTypesResult, this, [&, label](const QVector<RestServiceI::PersonType> value){
        label->close();
        delete label;
        m_vecPersonTypes = value;
        // 3.5 add
        if (value.count() > 0)
        {
            m_pBtnOperation = new QPushButton(tr("Select all"));
            m_pBtnOperation->setProperty(MNFaceTypeCheckedTag, false);
            m_pBtnOperation->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tab-noselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
            connect(m_pBtnOperation, SIGNAL(clicked(bool)),
                    this, SLOT(onBtnOperationClicked()));
            m_pFaceTypesHLay->addWidget(m_pBtnOperation);
            m_pFaceTypesHLay->addSpacing(10);
        }
        auto iter = value.begin();
        QPushButton *m_btnFaceType = Q_NULLPTR;
        for (iter; iter != value.end(); ++iter)
        {
#if 0
            m_faceLibBar->addTab(iter->strTypeName);
#endif
            m_btnFaceType = new QPushButton(iter->strTypeName);
            m_btnFaceType->setProperty(MNFaceTypeCheckedTag, false);
            m_btnFaceType->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tab-noselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
            m_pFaceTypesHLay->addWidget(m_btnFaceType);
            connect(m_btnFaceType, &QPushButton::clicked, this, [this, m_btnFaceType]{
                updateBtnPersonTypeStyle(m_btnFaceType);
            });
            m_lstFaceLibTypes.append(m_btnFaceType);
        }
        m_pFaceTypesHLay->addStretch();
        //slotAddRow(value);
    });
    serviceI->queryPersonTypes();
    label->show(500);
}

void MultipleFaceAnalysis::updatePersonTypesState()
{
    bool bChecked = m_pBtnOperation->property(MNFaceTypeCheckedTag).toBool();
    for (auto *tempBtn : m_lstFaceLibTypes)
    {
        tempBtn->setProperty(MNFaceTypeCheckedTag, !bChecked);
        qDebug() << "MNFaceTypeCheckedTag" << tempBtn->property(MNFaceTypeCheckedTag);
    }
    for (auto *btn : m_lstFaceLibTypes)
    {
        updateBtnPersonTypeStyle(btn);
    }
}

void MultipleFaceAnalysis::updateBtnPersonTypeStyle(QPushButton *btn)
{
    if (!btn->property(MNFaceTypeCheckedTag).toBool())
    {
        btn->setIcon(QIcon("images/portraitlibrary/icon_selected.png"));
        btn->setProperty(MNFaceTypeCheckedTag, true);
        btn->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tabselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
    }
    else
    {
        btn->setIcon(QIcon(""));
        btn->setProperty(MNFaceTypeCheckedTag, false);
        btn->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tab-noselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
    }
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
        tempLab->setPixmap(QPixmap::fromImage(itemData.uploadImg).scaled(57, 80));
        tempLab->setAlignment(Qt::AlignCenter);
        m_tableW->setCellWidget(m_tableW->rowCount() - 1, UploadImgCol, tempLab);
#endif
        int columnIndex = 1;
        //QLabel *captureLab;
        QWidget *captureWgt = Q_NULLPTR;
        QLabel *captureLab = Q_NULLPTR;
        QLabel *idLab = Q_NULLPTR;
        QLabel *nameLab = Q_NULLPTR;
        QLabel *simiLab = Q_NULLPTR;
        QHBoxLayout *hLay = Q_NULLPTR;
        QVBoxLayout *vLay = Q_NULLPTR;
        QWidget *captureContainer = Q_NULLPTR;
        QHBoxLayout *hContainerLay = Q_NULLPTR;
        for (const RestServiceI::MNCaptureItem &captureitem : itemData.captureItems)
        {
            captureContainer = new QWidget;
            //captureContainer->setStyleSheet("background-color:black;");
            hContainerLay = new QHBoxLayout;
            hContainerLay->setMargin(0);
            hContainerLay->setSpacing(0);
            hContainerLay->setAlignment(Qt::AlignCenter);
            captureContainer->setLayout(hContainerLay);
            captureWgt = new QWidget;
            hContainerLay->addWidget(captureWgt);
            captureWgt->setStyleSheet("font-size:10px;color:#7E8CB1;font-family:PingFang SC Regular;height:50px;");
            hLay = new QHBoxLayout;
            hLay->setMargin(0);
            hLay->setAlignment(Qt::AlignCenter);
            hLay->setSpacing(10);
            captureWgt->setLayout(hLay);
            vLay = new QVBoxLayout;

            captureLab = new QLabel;
            captureLab->setPixmap(QPixmap::fromImage(captureitem.captureImg).scaled(46, 46));
            hLay->addWidget(captureLab);
            hLay->addLayout(vLay);
            idLab = new QLabel(QString(tr("%1%2")).arg("ID:").arg(QString::number(captureitem.nPersonId)));
            vLay->addWidget(idLab);
            nameLab = new QLabel(QString("%1%2").arg(tr("Name:")).arg(captureitem.strPersonName));
            qDebug() << "name" << nameLab->text();
            vLay->addWidget(nameLab);
            simiLab = new QLabel(QString(tr("%1%2")).arg("Similarity:").arg(QString::number(captureitem.dSimilarity)));
            vLay->addWidget(simiLab);
            m_tableW->setCellWidget(m_tableW->rowCount() - 1, columnIndex, captureWgt);
#if 0
            captureLab = new QPushButton;
            captureLab->setStyleSheet("background-color:transparent;border:0px;");
            captureLab->setProperty(CaptureImgTag, "true");
            captureLab->setIcon(QPixmap::fromImage(captureitem.captureImg).scaled(46, 46));
            captureLab->setIconSize(QSize(100, 100));
            //captureLab->setAlignment(Qt::AlignCenter);
            m_tableW->setCellWidget(m_tableW->rowCount() - 1, columnIndex, captureLab);
#endif
            columnIndex++;
        }
        if ((0 == itemData.captureItems.count())
                && !itemData.bSuccess)
        {
            QTableWidgetItem *errItem = new QTableWidgetItem(itemData.strErrMsg);
            m_tableW->setItem(m_tableW->rowCount() - 1, Capture1, errItem);
            errItem->setTextAlignment(Qt::AlignCenter);
        }
        //auto captureItems = itemData.captureItems;

    }
}
