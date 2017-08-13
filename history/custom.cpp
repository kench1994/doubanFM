#include "custom.h"
#include "fm.h"
#include "ui_custom.h"
#include <QMouseEvent>
#include <Qpalette>
#include "douban_types.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>



extern QString _USER_ID,
               _USER_ICON,
               _USER_TOKEN,
               _USER_EXPIRE,
               _SID,
               _TYPE,
               _NOW_CHANNEL;
extern  bool   b_HasLogin;

unsigned int ui_IconNum;

custom::custom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::custom),
    doubanfm(DoubanFM::getInstance())
{
    ui->setupUi(this);

    /*set Ui*/
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->email->setStyleSheet("font: 75 11pt 华文细黑;border :1px ;color: rgb(255, 255, 255);background : (0x00,0xff,0x00,0x00)");
    ui->password->setStyleSheet("font: 75 11pt 华文细黑;border :1px ;color: rgb(255, 255, 255);background : (0x00,0xff,0x00,0x00)");
    ui->loginButton->setFlat(true);
    ui->cancelButton->setFlat(true);
   userInfoGetter = new QNetworkAccessManager(this);
   userIconGetter = new QNetworkAccessManager(this);
   connect(userInfoGetter, SIGNAL(finished(QNetworkReply*)), this, SLOT(userInfoReply(QNetworkReply*)));
   connect(userIconGetter, SIGNAL(finished(QNetworkReply*)), this, SLOT(userIconReply(QNetworkReply*)));
//   ui->fakeLabel->setVisible(false);
//   connect(&user,SIGNAL(userIconReady()),this,SLOT(setUserIcon()));
}

void custom::userInfoReply(QNetworkReply *reply){
    QByteArray data = reply->readAll();
    GetDATA = QString::fromUtf8(data);
//    qDebug()<<GetDATA;
    FindUserInfo(GetDATA);
//    userIconGetter->get(QNetworkRequest(QUrl(API_ICON+_USER_ID+".jpg")));
    userIconGetter->get(QNetworkRequest(QUrl(_USER_ICON)));
//    setUserIcon();
    reply->deleteLater();
}

void custom::setUserIcon(){
//    for(int i = 0 ;i<3;i++)
    ui->userIconLabel->setPixmap(QPixmap("icon.jpg").scaled(170, 170, Qt::KeepAspectRatioByExpanding));
}


/*成功登录后回吊*/
void custom::LoginDeal(){
    /*
    //获取网络图片
    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://www.oschina.net/img/logo_s2.png")));
    */
    b_HasLogin = true;
    userInfoGetter->get(QNetworkRequest(QUrl(FM_API_ADDR + _USER_ID)));
    //然后禁止用户输入
    this->close();
}

void custom::FindUserInfo(QString userInfo){
    int pos1,pos2=0,pos3;
    int counter = 0;
    //Search icon
    pos1=pos2;
    for(;pos1<userInfo.length();pos1++){
        if(userInfo[pos1]=='l'&&userInfo[pos1+1]=='i'&&userInfo[pos1+2]=='n'&&userInfo[pos1+3]=='k' ){
            pos1+=11;
            if(counter<2)   counter++;
            else     break;
        }
    }

    for(pos2=pos1;pos2<userInfo.length();pos2++){
        if(userInfo[pos2]=='i'&&userInfo[pos2+1]=='c'&&userInfo[pos2+2]=='o'&&userInfo[pos2+3]=='n' ){
             pos2+=6;
             break;
         }
     }
    for(pos3=pos2;pos3<userInfo.length();pos3++){
        if(userInfo[pos3]=='.'&&userInfo[pos3+1]=='j'&&userInfo[pos3+2]=='p'&&userInfo[pos3+3]=='g' ){
             pos3+=4;
             break;
         }
     }

    _USER_ICON = userInfo.mid(pos1,pos2-pos1) + "l" + userInfo.mid(pos2,pos3-pos2);//经过多次实验得出
//    qDebug()<<_USER_ICON;
}

void custom::userIconReply(QNetworkReply *reply){
    QPixmap pix;
    QByteArray data = reply->readAll();
    pix.loadFromData(data, "JPG");
    pix.save("icon.jpg", "JPG", 100);
    reply->deleteLater();
    ui->userIconLabel->setPixmap(QPixmap("icon.jpg").scaled(170, 170, Qt::KeepAspectRatioByExpanding));
}

custom::~custom(){
    delete ui;
    delete userIconGetter;
    delete userInfoGetter;
    delete userManager;
}


void custom::mousePressEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton){
        m_WindowPos = this->pos();
        m_MousePos = lpEvent->globalPos();
        this->m_MousePressed = true;
    }
}

void custom::mouseReleaseEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton)
    {
        this->m_MousePressed = false;
    }
}

void custom::mouseMoveEvent(QMouseEvent *lpEvent){
    if (m_MousePressed)
    {
        this->move(m_WindowPos + (lpEvent->globalPos() - m_MousePos));
    }
}



void custom::on_cancelButton_clicked(){
    this->close();
}

void custom::test(){
    qDebug()<<"this is kench testing";
}

void custom::on_loginButton_clicked(){
    custom *user = new custom;
    connect(user, SIGNAL(loginSucceed()), this,SLOT(LoginDeal()));
    QString email = ui->email->text();
    QString pwd = ui->password->text();
    if (email.size() == 0 || pwd.size() == 0) return;
    user->Login(email, pwd);
}

void custom::Login(const QString &email, const QString &password){
    QString args = QString("app_name=radio_desktop_win&version=100")
            + QString("&email=") + email
            + QString("&password=") + QUrl::toPercentEncoding(password);
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(args.length()));
    request.setUrl(QUrl(FM_LOGIN));

    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

//    if (!userManager) {
    userManager = new QNetworkAccessManager(this);
    connect(userManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(onReceivedAuth(QNetworkReply*)));
//    }
    qDebug() << "Login with " << email;
    userManager->post(request, args.toLatin1());
}



void custom::onReceivedAuth(QNetworkReply *reply) {

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString all = codec->toUnicode(reply->readAll());
    QJsonParseError parseerr;
    QVariant result = QJsonDocument::fromJson(all.toUtf8(), &parseerr).toVariant();
    if (parseerr.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        if (obj["r"].toInt() != 0) {
            qDebug() << Q_FUNC_INFO << "Err" << obj["err"].toString();
//            errInfo = obj["err"].toString();
//            qDebug()<<errInfo;
//            ui->warningLabel->setText(errInfo);
//            emit loginFailed(obj["err"].toString());
//            this->_user.reset();
            reply->deleteLater();
            return;
        }
    _USER_ID = obj["user_id"].toString();
    _USER_EXPIRE = obj["expire"].toString();
    _USER_TOKEN = obj["token"].toString();
    qDebug()<<"user_id:"<<_USER_ID<<" expire:"<<_USER_EXPIRE<<" token:"<<_USER_TOKEN;

//        _user.user_id = obj["user_id"].toString();

//        _user->user_id = obj["user_id"].toString();
//        _user->expire = obj["expire"].toString();
//        _user->token = obj["token"].toString();
//        _user->user_name = obj["user_name"].toString();
//        _user->email = obj["email"].toString();

        emit this->loginSucceed();
    }

    reply->deleteLater();
}

