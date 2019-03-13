#include "portraitsearch.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QComboBox>

#include "pageindicator.h"
#include "nodatatip.h"
#include "waitinglabel.h"
#include "informationdialog.h"

const char* FaceTypeCheckedTag = "FaceTypeChecked";
PortraitSearch::PortraitSearch(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Single Search"));

    init(); // init ui
    setUserStyle(userStyle());
}

void PortraitSearch::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if (0 == s)
    {
        f = font();
        f.setFamily("PingFang SC Regular");
        setFont(f);

        pal = this->palette();
        pal.setColor(QPalette::Foreground, QColor("#313745"));

        m_btnImg->setStyleSheet("QPushButton{"
                                "background-color: transparent;"
                                "}");
        QString commStyle = "font-size:16px;color:#7E8CB1;font-family:PingFang SC Regular;";
        m_pLabSimilary->setStyleSheet(commStyle);
        // 3.1 add
        m_pLabDbTitle->setStyleSheet(commStyle);
        m_pLabSimilary->setStyleSheet(commStyle);
        m_pTxtSimilary->setStyleSheet("width:180px;max-width:250px;height:34px;border-image:url(images/portraitlibrary/text.png);font-size:16px;color:white;font-family:PingFang SC Regular;");
        m_pFaceTypesWgt->setStyleSheet(".QWidget{border-image:url(images/portraitlibrary/barbg.png);background-color:transparent;min-width:400px;height:45px;max-height:45px;}");
        m_pDbType->setStyleSheet("QComboBox{width:180px;max-width:180px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;font-size:16px;}"
                                 "QComboBox::drop-down{"
                                 "subcontrol-position: center right;border-image: url(images/portraitlibrary/icon_arrow.png);width:8px;height:10px;subcontrol-origin: padding;margin-right:5px;"
                                 "}"
                                 "QComboBox QAbstractItemView{"
                                 "background-color:#282D38;"
                                 "selection-color: white;"
                                 "outline: 0px;"
                                 "selection-background-color: #4741F2;"
                                 "}"
                                 "QComboBox QAbstractItemView::item {color:white;height:25px; min-height: 25px; border-image:url(images/portraitlibrary/text.png);}");
        //qDebug() << m_pDbType->styleSheet();
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
                "QTableView QTableCornerButton{"
                "background: #41495C;border:0px;"
                "}"
                "QTableView QTableCornerButton::section{"
                "background: #41495C;border:0px;"
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
                                    "border: 0px;"
                                    "color:white;"
                                    "font-size:12px;width:99px;height:42px;font-family:Regular;"
                                    "border-image:url(images/portraitlibrary/search.png);"
                                    "}"
                                    "QPushButton:pressed{"
                                    "padding: 1px;"
                                    "}");

        m_pDataTip->setUserStyle(s);
}
}

void PortraitSearch::slotAddImage(QImage &img)
{
    m_faceImg = img;
    m_btnImg->setIcon(QPixmap::fromImage(m_faceImg).scaled(m_btnImg->iconSize()));
}

bool PortraitSearch::event(QEvent *event)
{
    if (m_bFirstLoading && (event->type() == QEvent::Show))
    {
        queryPersonTypes();
        m_bFirstLoading = false;
        return true;
    }
    return WidgetI::event(event);
}

void PortraitSearch::onBtnSearchClicked()
{
    if (0 == m_vecPersonTypes.size())
    {
        InformationDialog infoDialog(m_tableW);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(tr("Please select the type of person"));
        infoDialog.exec();
        return;
    }
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
        //m_faceLibBar->setEnabled(true);
        m_pDataTip->show();
    });
    connect(serviceI, &RestServiceI::sigPortraitLibCompResult, this, [&,label](const QVector<RestServiceI::PortraitLibCompItem> value){
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
        //m_faceLibBar->setEnabled(true);
        m_pBtnSearch->setEnabled(true);
        m_pageIndicator->setEnabled(true);
        if (value.isEmpty())
        {
            m_pDataTip->show();
            return;
        }
    });
    RestServiceI::PortraitLibCompArgs args;
    args.image = m_faceImg;
    args.bRequireBase64 = true;
    //args.libType = m_vecPersonTypes[m_faceLibBar->currentIndex()].strTypeNo;
    QString strTypes;
    int index = 0;
    for (auto btnTypeItem : m_lstFaceLibTypes)
    {
        if (btnTypeItem->property(FaceTypeCheckedTag).toBool())
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
    args.limit = 100;
    args.sourceType = (0 == m_pDbType->currentIndex()) ? tr("2") : tr("2");
    serviceI->portraitLibCompSearch(args);
    label->show(500);
    //m_faceLibBar->setEnabled(false);
    m_pBtnSearch->setEnabled(false);
    m_pageIndicator->setEnabled(false);
    m_pDataTip->hide();
    m_tableW->model()->removeRows(0, m_tableW->rowCount());
}

void PortraitSearch::onBtnImgClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Add pictures"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("Images (*.png *.xpm *.jpg)"));
    m_faceImg = QImage(filePath);
    if (filePath.isEmpty() || m_faceImg.isNull())
    {
        m_btnImg->setIcon(m_btnImg->property("default-pix").value<QPixmap>().scaled(m_btnImg->iconSize()));
        qSwap(QImage(), m_faceImg);
    }
    else
    {
        m_btnImg->setIcon(QPixmap::fromImage(m_faceImg).scaled(m_btnImg->iconSize()));
    }
}

void PortraitSearch::onSectionClicked(int index)
{
    static bool isDescendingOrder = false;
    if (isDescendingOrder)
    {
        m_tableW->sortByColumn(index, Qt::AscendingOrder);
    }
    else
    {
        m_tableW->sortByColumn(index, Qt::DescendingOrder);
    }
    isDescendingOrder = !isDescendingOrder;
}

void PortraitSearch::onBtnOperationClicked()
{
    updateBtnPersonTypeStyle(m_pBtnOperation);
    if (!m_pBtnOperation->property(FaceTypeCheckedTag).toBool())
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

void PortraitSearch::init()
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->setSpacing(20);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setMargin(0);
    m_btnImg = new QPushButton;
    connect(m_btnImg, SIGNAL(clicked(bool)), this, SLOT(onBtnImgClicked()));
    m_btnImg->setToolTip(tr("Add pictures"));
    m_btnImg->setFixedSize(100, 100);
    m_btnImg->setIconSize(QSize(m_btnImg->size()));
    m_btnImg->setFocusPolicy(Qt::NoFocus);
    QPixmap imgPix("images/portraitlibrary/uploadbg.png");
    m_btnImg->setIcon(imgPix.scaled(m_btnImg->iconSize()));
    m_btnImg->setProperty("default-pix",imgPix);
    QCursor curSor = cursor();
    curSor.setShape(Qt::PointingHandCursor);
    m_btnImg->setCursor(curSor);
    hlay->addWidget(m_btnImg);

    // top right area
    QWidget *topRigWgt = new QWidget;
    hlay->addWidget(topRigWgt);
    QVBoxLayout *topRighVlay = new QVBoxLayout;
    topRighVlay->setMargin(0);
    topRigWgt->setLayout(topRighVlay);
    QHBoxLayout *topHlay = new QHBoxLayout;
    topHlay->setSpacing(0);

    // 3.1 edit
    m_pFaceTypesWgt = new QWidget;
    m_pFaceTypesHLay = new QHBoxLayout;
    m_pFaceTypesHLay->setSpacing(5);
    m_pFaceTypesHLay->setMargin(2);
    m_pFaceTypesWgt->setLayout(m_pFaceTypesHLay);
    topHlay->addWidget(m_pFaceTypesWgt);
    m_pBtnSearch = new QPushButton(tr("Search"));
    connect(m_pBtnSearch, SIGNAL(clicked(bool)),
            this, SLOT(onBtnSearchClicked()));
    topHlay->addSpacing(10);
    topHlay->addWidget(m_pBtnSearch);
    topHlay->addStretch();
    QHBoxLayout *rightTopHlay = new QHBoxLayout;
    rightTopHlay->setSpacing(15);
    topRighVlay->addLayout(rightTopHlay);
    topRighVlay->addLayout(topHlay);

    m_pLabSimilary = new QLabel(tr("Similarity"));
    rightTopHlay->addWidget(m_pLabSimilary);
    m_pTxtSimilary = new QLineEdit(tr("0.3"));
    m_pTxtSimilary->setValidator(new QDoubleValidator(1, 0.1, 2, this));
    rightTopHlay->addWidget(m_pTxtSimilary);
    m_pLabDbTitle = new QLabel(tr("Database"));
    rightTopHlay->addWidget(m_pLabDbTitle);
    rightTopHlay->addSpacing(5);
    m_pDbType = new QComboBox;
    rightTopHlay->addWidget(m_pDbType);
    //m_pDbType->addItem(tr("mark"));
    m_pDbType->addItem(tr("regist"));

    rightTopHlay->addStretch();
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay);

    // content area
    m_tableW = new QTableWidget;
    mainLay->addWidget(m_tableW);
    m_tableW->setIconSize(QSize(112,112));
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
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("Photo") << tr("Name") << tr("ID") << tr("Similarity") << tr("Type") << tr("Type")); // Big type and small type.  << tr("Type"))
    m_tableW->setColumnWidth(ImageCol, 180);
    m_tableW->horizontalHeader()->setSortIndicatorShown(true);
    m_tableW->horizontalHeader()->setMinimumHeight(60);
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
}

void PortraitSearch::slotAddRow(QVector<RestServiceI::PortraitLibCompItem> info)
{
    if (info.isEmpty())
    {
        m_pDataTip->show();
    }
    for (const RestServiceI::PortraitLibCompItem &itemData : info)
    {
        m_tableW->insertRow(m_tableW->rowCount());
        QTableWidgetItem *item = new QTableWidgetItem;
#if 0
        item->setIcon(QPixmap::fromImage(m_faceImg));
        item->setTextAlignment(Qt::AlignCenter);
        //item->setData(Qt::UserRole,itemData.faceImg);
        m_tableW->setItem(m_tableW->rowCount() - 1,ImageCol,item);
#else
        QLabel *tempLab = new QLabel;
        tempLab->setPixmap(QPixmap::fromImage(itemData.faceImg).scaled(80, 80));
        tempLab->setAlignment(Qt::AlignCenter);
        m_tableW->setCellWidget(m_tableW->rowCount() - 1, ImageCol, tempLab);
#endif

        item = new QTableWidgetItem;
        item->setText(itemData.strPersonName);
        //item->setData(Qt::UserRole,itemData.strPersonName);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,PersonNameCol,item);

        item = new QTableWidgetItem;
        item->setText(QString::number(itemData.nPersonId));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,PersonIDCol,item);

        item = new QTableWidgetItem;
        QString similarityStr;
        similarityStr.setNum(itemData.dSimilarity * 100,'g',4);
        item->setText(similarityStr + '%');
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,SimilarityCol,item);

        item = new QTableWidgetItem;
        item->setText(itemData.strBigType);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,MainPersonType,item);

        item = new QTableWidgetItem;
        item->setText(itemData.strSubType);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,SubPersonType,item);
    }
}

void PortraitSearch::queryPersonTypes()
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
            m_pBtnOperation->setProperty(FaceTypeCheckedTag, false);
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
            m_btnFaceType = new QPushButton(iter->strTypeName);
            m_btnFaceType->setProperty(FaceTypeCheckedTag, false);
            m_btnFaceType->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tab-noselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
            m_pFaceTypesHLay->addWidget(m_btnFaceType);
            connect(m_btnFaceType, &QPushButton::clicked, this, [this, m_btnFaceType]{
                updateBtnPersonTypeStyle(m_btnFaceType);
            });
            m_lstFaceLibTypes.append(m_btnFaceType);
        }
        m_pFaceTypesHLay->addStretch();
    });
    serviceI->queryPersonTypes();
    label->show(500);
}

void PortraitSearch::updatePersonTypesState()
{
    bool bChecked = m_pBtnOperation->property(FaceTypeCheckedTag).toBool();
    for (auto *tempBtn : m_lstFaceLibTypes)
    {
        tempBtn->setProperty(FaceTypeCheckedTag, !bChecked);
        qDebug() << "FaceTypeCheckedTag" << tempBtn->property(FaceTypeCheckedTag);
    }
    for (auto *btn : m_lstFaceLibTypes)
    {
        updateBtnPersonTypeStyle(btn);
    }
}

void PortraitSearch::updateBtnPersonTypeStyle(QPushButton *btn)
{
    if (!btn->property(FaceTypeCheckedTag).toBool())
    {
        btn->setIcon(QIcon("images/portraitlibrary/icon_selected.png"));
        btn->setProperty(FaceTypeCheckedTag, true);
        btn->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tabselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
    }
    else
    {
        btn->setIcon(QIcon(""));
        btn->setProperty(FaceTypeCheckedTag, false);
        btn->setStyleSheet("QPushButton{border-image:url(images/portraitlibrary/tab-noselected.png);color:#7E8CB1;font-size:12px;font-family:PingFang SC Regular;width:120px;height:40px;}");
    }
}
