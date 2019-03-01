#include "portraitsearch.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardPaths>

#include "pageindicator.h"
#include "nodatatip.h"
#include "waitinglabel.h"
#include "informationdialog.h"

const char* FaceLibTypeTag = "FaceLibType";
PortraitSearch::PortraitSearch(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Portrait search"));

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
        QString commStyle = "font:16px;color:#7E8CB1;font-family:PingFang SC Regular;";
        m_pLabID->setStyleSheet(commStyle);
        m_txtID->setStyleSheet("width:150px;max-width:250px;height:34px;border-image:url(images/portraitlibrary/text.png);color:white;font-family:PingFang SC Regular;");
        m_pLabName->setStyleSheet(commStyle);
        m_pTxtName->setStyleSheet(m_txtID->styleSheet());
        m_faceLibBar->setStyleSheet("QTabBar{border-image:url(images/portraitlibrary/barbg.png);font-size:12px;background-color:transparent;}"
                                    "QTabBar::tab{border-image:url(images/portraitlibrary/tab-noselected.png);width:120px;height:40px;color:#7E8CB1;"
                                    "margin-right:2px;margin-left:2px;margin-top:2px;margin-bottom:2px;font-family:PingFang SC Regular;"
                                    "}"
                                    "QTabBar::tab:hover { \
                                    border-image:url(images/portraitlibrary/tabselected.png); \
                                    color:#7E8CB1; \
                                    }"
                                    "QTabBar::tab:selected { \
                                    border-image:url(images/portraitlibrary/tabselected.png); \
                                    color: white; \
                                }");
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
                                    "background-color: rgb(83,77,251);"
                                    "color: white;"
                                    "border-radius: 6px;"
                                    "font-size: 18px;"
                                    "}"
                                    "QPushButton:pressed{"
                                    "padding: 2px;"
                                    "background-color: #312DA6;"
                                    "}");

        m_pDataTip->setUserStyle(s);
        }
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
        m_faceLibBar->setEnabled(true);
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
        m_faceLibBar->setEnabled(true);
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
    args.libType = m_vecPersonTypes[m_faceLibBar->currentIndex()].strTypeNo;
    m_strBigPersonType = m_faceLibBar->tabText(m_faceLibBar->currentIndex());
    args.similarity = 0.3;
    args.limit = 100;
    if (!m_txtID->text().isEmpty())
    {
        args.nPersonId = m_txtID->text().toInt();
    }
    else
    {
        args.nPersonId = 0;
    }
    args.strPersonName = m_pTxtName->text();
    serviceI->portraitLibCompSearch(args);
    label->show(500);
    m_faceLibBar->setEnabled(false);
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

void PortraitSearch::init()
{
    // init face librarty types;
#if 0
    m_mapFaceLibTypes.insert(WhiteList, tr("White list"));
    m_mapFaceLibTypes.insert(BlackList, tr("Black list"));
    m_mapFaceLibTypes.insert(TagLibrary, tr("Tag library"));
    m_mapFaceLibTypes.insert(ImmigrationLibrary, tr("Immigration"));
    m_mapFaceLibTypes.insert(IDCenter, tr("ID center"));
#endif
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    m_btnImg = new QPushButton;
    connect(m_btnImg, SIGNAL(clicked(bool)), this, SLOT(onBtnImgClicked()));
    m_btnImg->setToolTip(tr("Add pictures"));
    m_btnImg->setFixedSize(84, 84);
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
    topRigWgt->setLayout(topRighVlay);
    QHBoxLayout *topHlay = new QHBoxLayout;
    topHlay->setSpacing(10);
    topRighVlay->addLayout(topHlay);
    //int nTypeCount = m_mapFaceLibTypes.size();
    m_faceLibBar = new QTabBar;
    m_faceLibBar->setDrawBase(false);
    //queryPersonTypes();
#if 0
    auto iter = m_mapFaceLibTypes.begin();
    for (iter; iter != m_mapFaceLibTypes.end(); ++iter)
    {
        m_faceLibBar->addTab(iter.value());
    }
#endif
    topHlay->addWidget(m_faceLibBar);
    topHlay->addStretch();
    QHBoxLayout *bottomHlay = new QHBoxLayout;
    bottomHlay->setSpacing(10);
    topRighVlay->addLayout(bottomHlay);
    m_pLabID = new QLabel(tr("ID"));
    bottomHlay->addWidget(m_pLabID);
    m_txtID = new QLineEdit;
    m_txtID->setValidator(new QIntValidator(0, 500000, this));
    bottomHlay->addWidget(m_txtID);
    m_pLabName = new QLabel(tr("Name"));
    bottomHlay->addWidget(m_pLabName);
    m_pTxtName = new QLineEdit;
    m_pTxtName->setMaxLength(50);
    bottomHlay->addWidget(m_pTxtName);
    m_pBtnSearch = new QPushButton(tr("Search"));
    connect(m_pBtnSearch, SIGNAL(clicked(bool)),
            this, SLOT(onBtnSearchClicked()));
    bottomHlay->addWidget(m_pBtnSearch);
    bottomHlay->addStretch();
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
        tempLab->setPixmap(QPixmap::fromImage(itemData.faceImg).scaled(112, 112));
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
        item->setText(m_strBigPersonType);
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
        auto iter = value.begin();
        for (iter; iter != value.end(); ++iter)
        {
            m_faceLibBar->addTab(iter->strTypeName);
        }
        //slotAddRow(value);
    });
    serviceI->queryPersonTypes();
    label->show(500);
}
