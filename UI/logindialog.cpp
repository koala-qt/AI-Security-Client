#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QApplication>
#include <QDialogButtonBox>
#include <QSettings>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include "service/restserviceconcureent.h"
#include "waitinglabel.h"
#include "logindialog.h"

#pragma execution_character_set("utf-8")
LoginDialog::LoginDialog(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    logoLabel_ = new QLabel;
    logoSpitL_ = new QLabel;
    logoTitleL_ = new QLabel(qApp->applicationName());
    logoBackW_ = new QWidget;
    ptnMinsized_ = new QPushButton(tr("-"));
    ptnClose_ = new QPushButton(tr("×"));
    hlay->setAlignment(Qt::AlignLeft);
    logoBackW_->setLayout(hlay);
    hlay->addWidget(logoLabel_);
    hlay->addWidget(logoSpitL_);
    hlay->addWidget(logoTitleL_);
    QHBoxLayout *topRightLay = new QHBoxLayout;
    topRightLay->addWidget(ptnMinsized_);
    topRightLay->addWidget(ptnClose_);
    topRightLay->setAlignment(Qt::AlignTop | Qt::AlignRight);
    hlay->addStretch();
    hlay->addLayout(topRightLay);
    mainLay->addWidget(logoBackW_);

    QGridLayout *centerLay = new QGridLayout;
    ipL_ = new QLabel(tr("ip"));
    ipEdit_ = new QLineEdit;
    ipEdit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    portL_ = new QLabel(tr("port"));
    portSpinBox_ = new QSpinBox;
    hostL_ = new QLabel(tr("Host"));
    hostEdit_ = new QLineEdit;
    userL_ = new QLabel(tr("user"));
    userEdit_ = new QLineEdit;
    userEdit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    pswL_ = new QLabel(tr("password"));
    pswEdit_ = new QLineEdit;
    pswEdit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    centerLay->addWidget(ipL_,0,0,1,1);
    centerLay->addWidget(ipEdit_,0,1,1,1);
    centerLay->addWidget(portL_,1,0,1,1);
    centerLay->addWidget(portSpinBox_,1,1,1,1);
    centerLay->addWidget(hostL_,2,0,1,1);
    centerLay->addWidget(hostEdit_,2,1,1,1);
    centerLay->addWidget(userL_,3,0,1,1);
    centerLay->addWidget(userEdit_,3,1,1,1);
    centerLay->addWidget(pswL_,4,0,1,1);
    centerLay->addWidget(pswEdit_,4,1,1,1);
    centerLay->setContentsMargins(20,0,20,0);
    centerLay->setAlignment(Qt::AlignCenter);
    mainLay->addLayout(centerLay);

    hlay = new QHBoxLayout;
    btnBox_ = new QDialogButtonBox;
    ptnSurce_ = btnBox_->addButton(tr("login"),QDialogButtonBox::AcceptRole);
    ptnCancell_ = btnBox_->addButton(tr("exit"),QDialogButtonBox::RejectRole);
    hlay->addStretch();
    hlay->addWidget(btnBox_);
    hlay->addStretch();
    hlay->setContentsMargins(0,0,0,10);
    mainLay->addLayout(hlay);
    mainLay->setSpacing(10);
    mainLay->setMargin(0);
    setLayout(mainLay);

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ptnMinsized_->setFixedSize(32,32);
    ptnClose_->setFixedSize(32,32);
    ipEdit_->setMinimumHeight(40);
    hostEdit_->setMinimumHeight(40);
    portSpinBox_->setMinimumHeight(40);
    userEdit_->setMinimumHeight(40);
    pswEdit_->setMinimumHeight(40);
    ptnSurce_->setMinimumHeight(35);
    ptnCancell_->setMinimumHeight(35);
    logoLabel_->setPixmap(QPixmap("images/appLogo.png"));
    logoSpitL_->setFrameStyle(QFrame::VLine | QFrame::Raised);
    logoSpitL_->setLineWidth(0);
    logoSpitL_->setMidLineWidth(1);
    logoSpitL_->setFixedHeight(30);
    QSettings setting("config.ini",QSettings::IniFormat);
    ipEdit_->setText(setting.value("Login/ip").toString());
    portSpinBox_->setValue(setting.value("Login/port").toInt());
    hostEdit_->setText(setting.value("Login/host").toString());
    hkUser_=setting.value("Login/HKuser").toString();
    userEdit_->setText(setting.value("Login/user").toString());
    QString hkpswOrigin = setting.value("Login/HKpsw").toString();
    QString psw;
    for(auto sc : hkpswOrigin){
        psw += sc.toLatin1() + 32;
    }
    hkPsw_ = psw;
    hkpswOrigin = setting.value("Login/psw").toString();
    psw.clear();
    for(auto sc : hkpswOrigin){
        psw += sc.toLatin1() + 32;
    }
    pswEdit_->setEchoMode(QLineEdit::Password);
    pswEdit_->setText(psw);
    connect(ptnMinsized_,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));
    connect(ptnClose_,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(ptnSurce_,SIGNAL(clicked(bool)),this,SLOT(slotSureBtnClicked()));
    connect(ptnCancell_,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    movieable_ = false;
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if(logoBackW_->geometry().contains(event->pos())){
        movieable_ = true;
        startPos_ = event->globalPos();
    }
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(movieable_){
        move(pos() + event->globalPos() - startPos_);
        startPos_ = event->globalPos();
    }
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QColor(Qt::blue).lighter());
    painter.setBrush(QColor(0,0,0,255));
    painter.drawRoundRect(rect().adjusted(0,0,-painter.pen().width(),-painter.pen().width()),5,5);
}

void LoginDialog::setUserStyle(int s)
{
    QPalette pal;
    QFont f = font();
    f.setPixelSize(16);
    setFont(f);

    pal = palette();
    pal.setColor(QPalette::Background,Qt::transparent);
    pal.setColor(QPalette::Foreground,Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
    if(s == 0){

        pal.setColor(QPalette::Background,QColor(52,162,255,85));
        logoBackW_->setPalette(pal);
        logoBackW_->setAutoFillBackground(true);

        pal = logoSpitL_->palette();
        pal.setColor(QPalette::Mid,Qt::white);
        logoSpitL_->setPalette(pal);

        ipEdit_->setStyleSheet("QLineEdit{"
                               "color:white;"
                               "border:1px solid rgb(86,159,213);"
                               "border-radius:5px;"
                               "background-color:transparent;"
                               "}");
        hostEdit_->setStyleSheet("QLineEdit{"
                               "color:white;"
                               "border:1px solid rgb(86,159,213);"
                               "border-radius:5px;"
                               "background-color:transparent;"
                               "}");
        userEdit_->setStyleSheet("QLineEdit{"
                               "color:white;"
                               "border:1px solid rgb(86,159,213);"
                               "border-radius:5px;"
                               "background-color:transparent;"
                               "}");
        pswEdit_->setStyleSheet("QLineEdit{"
                               "color:white;"
                               "border:1px solid rgb(86,159,213);"
                               "border-radius:5px;"
                               "background-color:transparent;"
                               "lineedit-password-character: 42;"
                               "}");
        ptnSurce_->setStyleSheet("QPushButton{"
                                 "background-color: rgb(83,77,251);"
                                 "color: white;"
                                 "border-radius: 6px;"
                                 "font-size: 18px;"
                                 "}"
                                 "QPushButton:pressed{"
                                 "padding: 2px;"
                                 "background-color: #312DA6;"
                                 "}");
        ptnCancell_->setStyleSheet("QPushButton{"
                                   "background-color: rgb(83,77,251);"
                                   "color: white;"
                                   "border-radius: 6px;"
                                   "font-size: 18px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "background-color: #312DA6;"
                                   "}");
        ptnClose_->setStyleSheet("QPushButton{"
                                 "background-color: rgb(83,77,251);"
                                 "color: white;"
                                 "border-radius: 6px;"
                                 "font-size: 18px;"
                                 "}"
                                 "QPushButton:pressed{"
                                 "padding: 2px;"
                                 "background-color: #312DA6;"
                                 "}");
        ptnMinsized_->setStyleSheet("QPushButton{"
                                    "background-color: rgb(83,77,251);"
                                    "color: white;"
                                    "border-radius: 6px;"
                                    "font-size: 18px;"
                                    "}"
                                    "QPushButton:pressed{"
                                    "padding: 2px;"
                                    "background-color: #312DA6;"
                                    "}");
        portSpinBox_->setStyleSheet("QSpinBox{"
                                 "padding-right: 15px;"
                                 "border-width: 3;"
                                 "background-color: transparent;"
                                 "border:1px solid rgb(86,159,213);"
                                 "border-radius: 5px;"
                                 "color: white;"
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
    }
}

void LoginDialog::autoLoginOnce(int timeDuration)
{
    QTimer::singleShot(timeDuration,this,[this]{
        if(userEdit_->text().isEmpty() || portSpinBox_->value() == 0 || ipEdit_->text().isEmpty() || pswEdit_->text().isEmpty() || !isEnabled()){
            return;
        }
        slotSureBtnClicked();
    });
}

void LoginDialog::slotSureBtnClicked()
{
    WaitingLabel *waitL = new WaitingLabel(this);
    waitL->setAttribute(Qt::WA_DeleteOnClose);
    waitL->setMovieFile("images/login.gif");
#if 0
    BLL::Worker *w = new BLL::ISBSDLLService(workerM_);
    ISBSServiceI *serI = dynamic_cast<ISBSServiceI*>(w);
    connect(serI,&ISBSServiceI::sigError,this,[this,waitL](QString msg){
        waitL->close();
        setEnabled(true);
        QMessageBox::information(this,tr("登录"),msg);
    });
    connect(serI,&ISBSServiceI::sigLogin,this,[this,waitL](int userHandle){
        userHandle_ = userHandle;
        BLL::Worker *w = new BLL::RestService(workerM_);
        RestServiceI *serI = dynamic_cast<RestServiceI*>(w);
        connect(serI,&RestServiceI::sigLoginInfo,this,[this,waitL](LoginInfo res){
            waitL->close();
            setEnabled(true);
            loginInfo_ = res;
            QSettings setting("config.ini",QSettings::IniFormat);
            QString psw;
            for(auto sc : hkPsw_){
                psw += sc.toLatin1() - 32;
            }
            setting.setValue("Login/HKpsw",psw);
            setting.setValue("Login/HKuser",hkUser_);
            psw.clear();
            for(auto sc : pswEdit_->text()){
                psw += sc.toLatin1() - 32;
            }
            setting.setValue("Login/user",userEdit_->text());
            setting.setValue("Login/psw",psw);
            accept();
        });
        connect(serI,&RestServiceI::sigError,this,[this,waitL](QString msg){
            waitL->close();
            setEnabled(true);
            QMessageBox::information(this,tr("登录"),msg);
        });
        serI->login(userEdit_->text(),pswEdit_->text());
        workerM_->startWorker(w);
    });
    ISBSServiceI::LoginArgs args;
    args.ip = ipEdit_->text();
    args.port = portSpinBox_->value();
    args.user = hkUser_;
    args.psw = hkPsw_;
    serI->login(args);
    workerM_->startWorker(w);
#else
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serI = factoryI->makeRestServiceI();
    connect(serI,&RestServiceI::sigResultState,this,[this,waitL](bool res){
        waitL->close();
        setEnabled(true);
        if(!res){
            QMessageBox::information(this,tr("登录"),tr("登录失败"));
            return;
        }
        QSettings setting("config.ini",QSettings::IniFormat);
        QString psw;
        for(auto sc : hkPsw_){
            psw += sc.toLatin1() - 32;
        }
        setting.setValue("Login/HKpsw",psw);
        setting.setValue("Login/HKuser",hkUser_);
        psw.clear();
        for(auto sc : pswEdit_->text()){
            psw += sc.toLatin1() - 32;
        }
        setting.setValue("Login/user",userEdit_->text());
        setting.setValue("Login/psw",psw);
        setting.setValue("Login/host",hostEdit_->text());
        accept();
    });
    connect(serI,&RestServiceI::sigError,this,[this,waitL](QString msg){
        waitL->close();
        setEnabled(true);
        QMessageBox::information(this,tr("登录"),msg);
    });
    RestServiceI::LoginParameter loginArgs;
    loginArgs.ip = ipEdit_->text();
    loginArgs.port = portSpinBox_->value();
    loginArgs.host = hostEdit_->text();
    loginArgs.userName = userEdit_->text();
    loginArgs.password = pswEdit_->text();
    loginArgs.authenticationMethod = 1;
    loginArgs.xprotectProductFamily = 0;
//    serI->login(loginArgs);
#endif
    waitL->show(500);
    setEnabled(false);
}
