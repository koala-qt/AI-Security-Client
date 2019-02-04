#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QSpinBox>
#include <QEvent>
#include <QPainter>
#include <QMessageBox>
#include "combinationpage.h"
#include "waitinglabel.h"
#include "service/restservice.h"

CombinationPage::CombinationPage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Combination search"));
    imageBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("Search"));
    similarityL_ = new QLabel(tr("Similarity"));
    queryCountL_ = new QLabel(tr("Query count"));
    positionL_ = new QLabel(tr("Position"));
    startTimeL_ = new QLabel(tr("Starting time"));
    endTimeL_ = new QLabel(tr("Ending time"));
    quanzhongL_ = new QLabel(tr("Tradeoff"));
    quanzhongSpin_ = new QSpinBox;
    similaritySpin_ = new QSpinBox;
    queryCountCombox_ = new QComboBox;
    cameraCombox_ = new QComboBox;
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    faceDataL_ = new QLabel(tr("Faces larger than similarity"));
    bodyDataL_ = new QLabel(tr("Combined score"));
    faceTable_ = new QTableWidget;
    bodyTable_ = new QTableWidget;
    faceDataBackW_ = new QWidget;
    bodyDataBackW_ = new QWidget;
    conditionBackW_ = new QWidget;

    QVBoxLayout *mainLay = new QVBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(imageBtn_,0,0,2,1);
    gridLay->addWidget(similarityL_,0,1,1,1);
    gridLay->addWidget(similaritySpin_,0,2,1,1);
    gridLay->addWidget(queryCountL_,0,3,1,1);
    gridLay->addWidget(queryCountCombox_,0,4,1,1);
    gridLay->addWidget(quanzhongL_,0,5,1,1);
    gridLay->addWidget(quanzhongSpin_,0,6,1,1);
    gridLay->addWidget(positionL_,1,1,1,1);
    gridLay->addWidget(cameraCombox_,1,2,1,1);
    gridLay->addWidget(startTimeL_,1,3,1,1);
    gridLay->addWidget(startTimeEdit_,1,4,1,1);
    gridLay->addWidget(endTimeL_,1,5,1,1);
    gridLay->addWidget(endTimeEdit_,1,6,1,1);
    gridLay->addWidget(searchBtn_,1,7,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    conditionBackW_->setLayout(gridLay);
    QHBoxLayout *hlay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(faceDataL_);
    vlay->addWidget(faceTable_);
    faceDataBackW_->setLayout(vlay);
    hlay->addWidget(faceDataBackW_);
    vlay = new QVBoxLayout;
    vlay->addWidget(bodyDataL_);
    vlay->addWidget(bodyTable_);
    bodyDataBackW_->setLayout(vlay);
    hlay->addWidget(bodyDataBackW_);
    mainLay->addWidget(conditionBackW_);
    mainLay->addLayout(hlay);
    mainLay->setMargin(0);
    setLayout(mainLay);

    QPixmap pix("images/person-face-back.png");
    imageBtn_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    imageBtn_->setFixedSize(92,92);
    imageBtn_->setIconSize(imageBtn_->size());
    imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
    imageBtn_->setProperty("default-pix",pix);
    faceDataBackW_->installEventFilter(this);
    bodyDataBackW_->installEventFilter(this);
    conditionBackW_->installEventFilter(this);
    similaritySpin_->setMinimumSize(120,44);
    similaritySpin_->setSuffix("%");
    queryCountCombox_->setMinimumHeight(44);
    queryCountCombox_->setMaximumWidth(250);
    quanzhongSpin_->setMinimumHeight(44);
    quanzhongSpin_->setSuffix("%");
    cameraCombox_->setMinimumHeight(44);
    cameraCombox_->setMaximumWidth(250);
    startTimeEdit_->setMinimumHeight(44);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setMinimumHeight(44);
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    searchBtn_->setMinimumSize(120,44);
    QVector<QPair<QString,int>> itemVec{qMakePair(tr("20"),20),
                qMakePair(tr("50"),50),qMakePair(tr("100"),100),
                qMakePair(tr("200"),200),qMakePair(tr("300"),300),
                qMakePair(tr("500"),500)};
    for(const QPair<QString,int> &value : itemVec){
        queryCountCombox_->addItem(value.first,value.second);
    }
}

bool CombinationPage::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *backW = qobject_cast<QWidget*>(watched);
    if((backW == faceDataBackW_ || backW == bodyDataBackW_ || backW == conditionBackW_) && event->type() == QEvent::Paint){
        QPainter p(backW);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,50));
        p.drawRoundedRect(backW->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return WidgetI::eventFilter(watched,event);
}

void CombinationPage::slotSearchBtnClicked()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        delete label;
        QMessageBox::information(this,objectName(),str);
        searchBtn_->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigSemanticSearch,this,[this,label](RestServiceI::SemanticReturnData &returnData){
        label->close();
        delete label;
        searchBtn_->setEnabled(true);
    });
    RestServiceI::CombinationSearchArgs args;
    args.cameraId = cameraCombox_->currentData().toString();
    args.startTime = startTimeEdit_->dateTime();
    args.endTime = endTimeEdit_->dateTime();
    args.img = imageBtn_->property("pixmap").value<QPixmap>().toImage();
    args.queryCount = queryCountCombox_->currentData().toInt();
    args.similarity = similaritySpin_->value() / (qreal)100;
    args.tradeoff = quanzhongSpin_->value() / (qreal)100;
    serviceI->combinationSearch(args);
    startWorker(worker);
    label->show(500);
    searchBtn_->setEnabled(false);
}
void CombinationPage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == s){
        pal = palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        setPalette(pal);

        faceTable_->setStyleSheet(
                    "QTableView{"
                    "selection-background-color: rgb(235,245,255);"
                    "background-color: transparent;"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgb(0,138,194);"
                    "}"
                    "QHeaderView::section{"
                    "background-color: rgb(28,143,227);"
                    "color: white;"
                    "border: none;"
                    "}"
                    "QTableWidget::Item{"
                    "border-bottom: 1px solid rgb(184,184,184);"
                    "}"
                    "QTableWidget::item:selected{"
                    "background-color: rgb(0,138,194,102);"
                    "color: blue;"
                    "}");
        bodyTable_->setStyleSheet(
                    "QTableView{"
                    "selection-background-color: rgb(235,245,255);"
                    "background-color: transparent;"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgb(0,138,194);"
                    "}"
                    "QHeaderView::section{"
                    "background-color: rgb(28,143,227);"
                    "color: white;"
                    "border: none;"
                    "}"
                    "QTableWidget::Item{"
                    "border-bottom: 1px solid rgb(184,184,184);"
                    "}"
                    "QTableWidget::item:selected{"
                    "background-color: rgb(0,138,194,102);"
                    "color: blue;"
                    "}");
        pal = faceTable_->palette();
        pal.setColor(QPalette::AlternateBase,QColor(235,245,249));
        faceTable_->setPalette(pal);
        bodyTable_->setPalette(pal);

        cameraCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 16px;"
                    "background-color: transparent;"
                    "border: 1px solid rgba(255, 255, 255, 1);"
                    "border-radius: 4px;"
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
        queryCountCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 16px;"
                    "background-color: transparent;"
                    "border: 1px solid rgba(255, 255, 255, 1);"
                    "border-radius: 4px;"
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
        searchBtn_->setStyleSheet("QPushButton{"
                                 "color: white;"
                                 "background-color: rgba(112, 112, 112, 1);"
                                 "}");
        similaritySpin_->setStyleSheet("QSpinBox{"
                                 "padding-right: 15px;"
                                 "border-width: 3;"
                                 "background-color: transparent;"
                                 "border:1px solid #CECECE;"
                                    "border-radius:6px;"
                                 "color: white;"
                                 "font-size: 18px;"
                                 "}"
                                 "QSpinBox::up-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: top right;"
                                 "width: 16px;"
                                 "border-image: url(images/on.png) 1;"
                                 "}"
                                 "QSpinBox::down-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: bottom right;"
                                 "width: 16px;"
                                 "border-image: url(images/under.png) 1;"
                                 "}");
        quanzhongSpin_->setStyleSheet("QSpinBox{"
                                 "padding-right: 15px;"
                                 "border-width: 3;"
                                 "background-color: transparent;"
                                 "border:1px solid #CECECE;"
                                    "border-radius:6px;"
                                 "color: white;"
                                 "font-size: 18px;"
                                 "}"
                                 "QSpinBox::up-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: top right;"
                                 "width: 16px;"
                                 "border-image: url(images/on.png) 1;"
                                 "}"
                                 "QSpinBox::down-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: bottom right;"
                                 "width: 16px;"
                                 "border-image: url(images/under.png) 1;"
                                 "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        imageBtn_->setStyleSheet("QPushButton{"
                                 "background-color: transparent;"
                                 "}");
    }
}
