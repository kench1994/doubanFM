#include "fm.h"
#include "ui_fm.h"
#include "douban_types.h"
#include "time.h"


//static const QString API_CHANNEL = "http://douban.fm/j/mine/playlist?&channel=";


static const QString API_TYPE = "?type=";//一开始进入豆瓣type取值为n
static const QString API_SID = "&sid=";
static const QString API_CHANNEL = "&channel=";
static const QString API_ADD = "&from=mainsite";



extern QString _USER_ID,
               _USER_ICON,
               _USER_TOKEN,
               _USER_EXPIRE,
               _SID,
               _TYPE,
               _NOW_CHANNEL;
extern  bool   b_HasLogin;
extern DoubanChannel DoubanChannels[39];
//extern qint32 array_at;

fm::fm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fm)
{

    ui->setupUi(this);
    /*set Ui*/
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255,0));
    this->setPalette(palette);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->playButton->setFlat(true);
    ui->nextButton->setFlat(true);
    ui->deleteButton->setFlat(true);
    ui->voiceButton->setFlat(true);
    ui->leftButton->setFlat(true);
    ui->rightButton->setFlat(true);
    ui->userButton->setFlat(true);
    ui->likeButton->setFlat(true);
//    ui->likeButton->setStyleSheet("QPushButton{border-image: url(:/image/heart.png);"
//                                  "border:0px 0px 0px 0px;}"
//                                  "QPushButton:pressed{border-image: url(:/image/heart-.png);}");

    /*Add New Player*/
    player=new QMediaPlayer();


    /*set Default Volume*/
    ui->volumeSlider->setValue(100);
    ui->volumeSlider->setMaximum(100);
    ui->volumeSlider->setMinimum(0);
    player->setVolume(100);


    /*set Player state*/
    b_HasLogin = false;
    b_PlayStatus=true;
    b_voiceON = true;
    _TYPE = "n";
//    DoubanSong.like = false;

    /*set GET*/
//    songManager = new QNetworkAccessManager(this);
//    connect(songManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(songReply(QNetworkReply*)));
    albumImg = new QNetworkAccessManager(this);
    connect(albumImg, SIGNAL(finished(QNetworkReply*)),this, SLOT(imgReply(QNetworkReply*)));
    songDeal = new QNetworkAccessManager(this);




    // title roll
    m_timer = new QTimer(this);
    connect(m_timer,  SIGNAL(timeout()),  this,  SLOT(scrollCaption()));


    /* set position */
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(durationChanged(qint64)));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),SLOT(setVolume(int)));
    connect(&channel,SIGNAL(changeChannel()),this,SLOT(slidChannel()));




    //timeslider
    ui->timeSlider->setRange(0, 0);


    //rand channel
//    srand((unsigned) time(NULL));
//    int temp = rand() % 26;
//    _NOW_CHANNEL = QString::number(temp);
    _NOW_CHANNEL = "0";//默认私人兆赫

    /*fm Load*/
    GetSongs();


}

fm::~fm()
{
    delete ui;
    delete player;
//    delete songManager;
    delete songDeal;
    delete m_timer;
}



void fm::setVolume(int position){
    player->setVolume(position);
}


void fm::mouseMoveEvent(QMouseEvent *lpEvent){
    if (b_MousePressed)
    {
        this->move(qp_WindowPos + (lpEvent->globalPos() - qp_MousePos));
    }
}


void fm::mousePressEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton)    {
        qp_WindowPos = this->pos();
        qp_MousePos = lpEvent->globalPos();
        this->b_MousePressed = true;
    }
}

void fm::mouseReleaseEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton)    {
        this->b_MousePressed = false;
    }
}

void fm::on_playButton_clicked(){
    if(b_PlayStatus == true){
        player->pause();
        ui->playButton->setStyleSheet("QPushButton{border-image: url(:/image/play.png);"
                                      "border:0px 0px 0px 0px;}"
                                      "QPushButton:pressed{border-image: url(:/image/play-.png);}");
        b_PlayStatus = false;
    }else{
        player->play();
        ui->playButton->setStyleSheet("QPushButton{border-image: url(:/image/pause.png);"
                                      "border:0px 0px 0px 0px;}"
                                      "QPushButton:pressed{border-image: url(:/image/pause-.png);}");
        b_PlayStatus = true;
    }
}

void fm::on_nextButton_clicked(){

    if(m_timer != nullptr)
        m_timer->stop();
    ui->playButton->setStyleSheet("QPushButton{border-image: url(:/image/pause.png);"
                                  "border:0px 0px 0px 0px;}"
                                  "QPushButton:pressed{border-image: url(:/image/pause-.png);}");
    b_PlayStatus = true;
//    qDebug()<<DoubanSong.sid;
    _TYPE = "s";
    GetSongs();
}

void fm::slidChannel(){
    _TYPE = "n";
    GetSongs();
}



void fm::on_likeButton_clicked(){
    //只在登录后有效
    if(b_HasLogin == true){
        if(DoubanSong.like == true){
            //设置图标要放到上面
            _TYPE = "u";
            ui->likeButton->setStyleSheet("QPushButton{border-image: url(:/image/heart.png);"
                                            "border:0px 0px 0px 0px;}"
                                            "QPushButton:pressed{border-image: url(:/image/heart-.png);}");
        }else{
            _TYPE = "r";
            ui->likeButton->setStyleSheet("QPushButton{border-image: url(:/image/heart-.png);"
                                          "border:0px 0px 0px 0px;}"
                                          "QPushButton:pressed{border-image: url(:/image/heart.png);}");
        }

        songUrl = API_SONGLIST + API_TYPE + _TYPE + API_SID + QString::number(DoubanSong.sid,10) + API_CHANNEL + _NOW_CHANNEL + API_ADD;
        qDebug()<<songUrl;
        songDeal->get(QNetworkRequest(songUrl));
    }else{
        DoubanSong.like = false;
        ui->likeButton->setStyleSheet("QPushButton{border-image: url(:/image/heart.png);"
                                        "border:0px 0px 0px 0px;}"
                                        "QPushButton:pressed{border-image: url(:/image/heart-.png);}");
    }
    _TYPE = "s";    //恢复默认设定
}

void fm::on_userButton_clicked(){
    user.show();
}


void fm::GetSongs(){
    //获取歌曲列表
//    if(b_HasLogin == true) {_TYPE = "n";getNewPlayList();}
//    else{
//    songUrl = API_SONGLIST + API_TYPE + _TYPE + API_SID + QString::number(DoubanSong.sid,10) + API_CHANNEL + _NOW_CHANNEL + API_ADD;
//    qDebug()<<songUrl;
//    songManager->get(QNetworkRequest(songUrl));}
    getNewPlayList();
}


//void fm::FindSongsList(QString Songs){


//    int pos1,pos2=0;
//    //search album
//    pos1=pos2;
//    for(;pos1<Songs.length();pos1++){
//         if(Songs[pos1]=='a'&&Songs[pos1+1]=='l'&&Songs[pos1+2]=='b'&&Songs[pos1+3]=='u'){
//             pos1+=13;
//             break;
//          }
//      }
//    for(pos2=pos1;pos2<Songs.length();pos2++){
//         if(Songs[pos2]=='_'&&Songs[pos2+1]=='e'&&Songs[pos2+2]=='x'&&Songs[pos2+3]=='t'){
//            pos2-=7;
//            break;
//         }
//    }
//    DoubanSong.album= Songs.mid(pos1,pos2-pos1);
//    qDebug()<<"album"<<DoubanSong.album;


//    pos1=pos2;
//    //Serach title
//    for(;pos1<Songs.length();pos1++){
//       if(Songs[pos1]=='i'&&Songs[pos1+1]=='t'&&Songs[pos1+2]=='l'&&Songs[pos1+3]=='e'){
//           pos1+=7;
//           break;
//        }
//    }
//    for(pos2=pos1;pos2<Songs.length();pos2++){
//       if(Songs[pos2]=='"'&&Songs[pos2+1]==','&&Songs[pos2+2]=='"'){
//          pos2-=1;
//          break;
//        }
//    }
//    DoubanSong.title= Songs.mid(pos1,pos2-pos1+1);
//    qDebug()<<"title:"<<DoubanSong.title;


//    //Search sid
////    pos1=pos2+1;
////    for(;pos1<Songs.length();pos1++){
////         if(Songs[pos1]=='"'&&Songs[pos1+1]=='s'&&Songs[pos1+2]=='i'&&Songs[pos1+3]=='d'){
//////             pos1+=4;
////             break;
////          }
////      }
////    for(pos2=pos1;pos2<Songs.length();pos2++){
////         if(Songs[pos2]=='s'&&Songs[pos2+1]=='h'&&Songs[pos2+2]=='a'&&Songs[pos2+3]=='2'){
//////            pos2-=3;
////            break;
////         }
////    }
////    bool ok;
////    DoubanSong.sid= (quint32)Songs.mid(pos1,pos2-pos1).toULong(&ok,10);
////    qDebug()<<DoubanSong.sid;//song.sid;


//    //Search Pic
//    pos1=pos2;
//    for(;pos1<Songs.length();pos1++){
//         if(Songs[pos1]=='u'&&Songs[pos1+1]=='r'&&Songs[pos1+2]=='e'&&Songs[pos1+3]=='"'){
//             pos1+=6;
//             break;
//          }
//      }
//    for(pos2=pos1;pos2<Songs.length();pos2++){
//         if(Songs[pos2]=='.'&&Songs[pos2+1]=='j'&&Songs[pos2+2]=='p'&&Songs[pos2+3]=='g'){
//            pos2+=4;
//            break;
//         }
//    }
//    DoubanSong.picture= Songs.mid(pos1,pos2-pos1);
//    DoubanSong.picture.replace(QString("\\"),QString(""));
//    qDebug()<<"picture:"<<DoubanSong.picture;



//        pos1=pos2;
//        //Search Artist

//        for(;pos1<Songs.length();pos1++){
//             if(Songs[pos1]=='"'&&Songs[pos1+1]=='n'&&Songs[pos1+2]=='a'&&Songs[pos1+3]=='m'){
//                 pos1+=8;
//                 break;
//              }
//          }
//        for(pos2=pos1;pos2<Songs.length();pos2++){
//             if(Songs[pos2]=='"'&&Songs[pos2+1]=='l'&&Songs[pos2+2]=='i'&&Songs[pos2+3]=='k'){
//                pos2-=4;
//                break;
//             }
//        }
//        DoubanSong.artist= Songs.mid(pos1,pos2-pos1);
////        DoubanSong.artist.replace(QString("\\"),QString(""));
//        qDebug()<<"artist:"<<DoubanSong.artist;


//        pos1=pos2;
//        //Serach url
//        for(;pos1<Songs.length();pos1++){
//           if(Songs[pos1]=='"'&&Songs[pos1+1]=='u'&&Songs[pos1+2]=='r'&&Songs[pos1+3]=='l'){
//               pos1+=7;
//               break;
//            }
//        }
//        for(pos2=pos1;pos2<Songs.length();pos2++){
//           if(Songs[pos2]=='.'&&Songs[pos2+1]=='m'&&Songs[pos2+2]=='p'&& (Songs[pos2+3]=='3' || Songs[pos2+3]=='4')){
//              pos2+=4;
//              break;
//            }
//        }
//        DoubanSong.url= Songs.mid(pos1,pos2-pos1);
//        DoubanSong.url.replace(QString("\\"),QString(""));
//        qDebug()<<"url:"<<DoubanSong.url;

//        DoubanSong.like = false;

//   _Play();
//}



void fm::_Play(){
    //

    player->setMedia(QUrl(DoubanSong.url));


    QNetworkRequest coverRequest;
    coverRequest.setUrl(QUrl(DoubanSong.picture));
    albumImg->get(coverRequest);

    if(b_HasLogin == false) DoubanSong.like = false;


    if(DoubanSong.like == true)     ui->likeButton->setStyleSheet("QPushButton{border-image: url(:/image/heart-.png);"
                                                                  "border:0px 0px 0px 0px;}"
                                                                  "QPushButton:pressed{border-image: url(:/image/heart.png);}");
    else     ui->likeButton->setStyleSheet("QPushButton{border-image: url(:/image/heart.png);"
                                           "border:0px 0px 0px 0px;}"
                                           "QPushButton:pressed{border-image: url(:/image/heart-.png);}");


    if((DoubanSong.title.length())>18){
        //scortitle
//        if(m_timer == nullptr){
//            m_timer = new QTimer(this);
//            connect(m_timer,  SIGNAL(timeout()),  this,  SLOT(scrollCaption()));}
        m_scrollCaptionStr = DoubanSong.title;
        m_timer->start(1000);
    }
    else  ui->titleLabel->setText(DoubanSong.title);


    ui->artistLabel->setText(DoubanSong.artist);
    player->play();
}

//void fm::songReply(QNetworkReply *reply){
//    if(b_HasLogin)  getNewPlayList();
//    else{
//    QByteArray bytes = reply->readAll();
//    /* it`s GET date*/
//    GetDATA = QString::fromUtf8(bytes);
////    qDebug()<<GetDATA;
//    FindSongsList(GetDATA);
//    reply->deleteLater();
//    }
//}




void fm::imgReply(QNetworkReply *reply){
    QPixmap pix;
    QByteArray data = reply->readAll();
    pix.loadFromData(data, "JPG");
    pix.save("cover.jpg", "JPG", 100);
    ui->picLabel->setPixmap(QPixmap("cover.jpg").scaled(160, 160, Qt::KeepAspectRatioByExpanding));
    reply->deleteLater();
}

void fm::scrollCaption(){
    static int nPos = 0;
    if (nPos > m_scrollCaptionStr.length()/2)    {
        nPos = 0;
    }
    ui->titleLabel->setText(m_scrollCaptionStr.mid(nPos));
    nPos++;
}


void fm::durationChanged(qint64 duration){
    ui->totalTime->setText(CovertTime(duration));
    ui->timeSlider->setRange(0, duration);
    allduration=duration;//直接获取该音频文件的总时长参数，单位为毫秒
}

QString fm::CovertTime(qint64 ti){
    qint64 temp=ti/1000;
    qint64 mm=temp/60;
    qint64 ss=temp%60;
    QString time="";
    if(mm<10)time="0"+QString::number(mm);
    else time=QString::number(mm);
    if(ss<10)time+=":0"+QString::number(ss);
    else time+=":"+QString::number(ss);
    return time;
}

void fm::positionChanged(qint64 position){
    ui->currentTime->setText(CovertTime(position));
    ui->timeSlider->setValue(position);
    UpdateTime(position);
//    qDebug()<<"pos:"<<position<<"dru"<<allduration;
    if(position == allduration){
        GetSongs();
    }
}

void fm::UpdateTime(qint64 time){
    //这3个参数分别代表了时，分，秒；60000毫秒为1分钟，所以分钟第二个参数是先除6000,第3个参数是直接除1s
    QTime total_time(0, (allduration/60000)%60, (allduration/1000)%60);
    QTime current_time(0, (time/60000)%60, (time/1000)%60);//传进来的time参数代表了当前的时间
    QString str = current_time.toString("mm:ss") + "/" + total_time.toString("mm:ss");
    qint64 previous=0;
}

void fm::getNewPlayList() {

    newList = new QNetworkAccessManager(this);
    connect(newList, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReceivedNewList(QNetworkReply*)));
    QNetworkRequest request;
    if(b_HasLogin == true){
        QString args = commitForm(_TYPE);
        request  = QNetworkRequest(QUrl(API_SONGLIST_LOG + args));
//        qDebug()<<API_SONGLIST_LOG + args;
    }else
        request = QNetworkRequest(QUrl(API_SONGLIST + API_TYPE + _TYPE + API_SID + QString::number(DoubanSong.sid,10) + API_CHANNEL + _NOW_CHANNEL + API_ADD));

    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    newList->get(request);
//    qDebug()<<DoubanSong.like;

}



void fm::onReceivedNewList(QNetworkReply *reply){
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        qWarning() << "Error occurred while receiving new list: " << reply->errorString();
        reply->deleteLater();
        return;
    }
    //为了能显示下载的网页中的中文，我们使用了QTextCodec 类对象，应用utf8编码
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString all = codec->toUnicode(reply->readAll());

    QList<DoubanFMSong> songs;
    QJsonParseError parseerr;
    QVariant result = QJsonDocument::fromJson(all.toUtf8(), &parseerr).toVariant();

    if (parseerr.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        if (obj["r"].toInt() != 0) {
            if (obj["err"].toString() == "expired") {
                qDebug() << Q_FUNC_INFO << "User expired. Relogin";
//                userReLogin();
            }
            else
                qDebug() << Q_FUNC_INFO << "Err" << obj["err"].toString();
            reply->deleteLater();
            return;
        }
        QVariantList songList = obj["song"].toList();
        foreach(const QVariant& item, songList) {
            QVariantMap song = item.toMap();

            DoubanSong.album = song["album"].toString();
            DoubanSong.picture = song["picture"].toString();
            DoubanSong.ssid = song["ssid"].toString();
            DoubanSong.artist = song["artist"].toString();
            DoubanSong.url = song["url"].toString();
            DoubanSong.company = song["company"].toString();
            DoubanSong.title = song["title"].toString();
            DoubanSong.public_time = song["public_time"].toString();
            DoubanSong.sid = song["sid"].toUInt();
            DoubanSong.aid = song["aid"].toUInt();
            DoubanSong.albumtitle = song["albumtitle"].toString();
            DoubanSong.like = song["like"].toBool();
            songs.push_back(DoubanSong);

//            qDebug()<<"album:"<<DoubanSong.album;
//            qDebug()<<"picture:"<<DoubanSong.picture;
//            qDebug()<<"ssid:"<<DoubanSong.ssid;
//            qDebug()<<"artist:"<<DoubanSong.artist;
//            qDebug()<<"url:"<<DoubanSong.url;
//            qDebug()<<"company:"<<DoubanSong.company;
//            qDebug()<<"title:"<<DoubanSong.title;
//            qDebug()<<"public_time:"<<DoubanSong.public_time;
//            qDebug()<<"sid:"<<DoubanSong.sid;
//            qDebug()<<"aid:"<<DoubanSong.aid;
//            qDebug()<<"albumtitle:"<<DoubanSong.albumtitle;
//            qDebug()<<"like:"<<DoubanSong.like;
        }
        _Play();

    }
//    emit receivedNewList(songs);
    reply->deleteLater();
}

//void fm::GetChannels(){


//}




//提交格式
QString fm::commitForm(QString type){
      QString args = QString("?app_name=radio_desktop_win&version=100")
    + QString("&user_id=") + _USER_ID               //为了编程方便基本暂时都设置成了全局变量
    + QString("&expire=") + _USER_EXPIRE
    + QString("&token=") + _USER_TOKEN
    + QString("&sid=") + QString::number(DoubanSong.sid,10)
    + QString("&h=")                                //非必选
    + QString("&channel=") + _NOW_CHANNEL           //非必选
    + QString("&kbps=")                             //非必选,暂时不填
    + QString("&type=") + type;                     //playing，歌曲正在播放，队列中还有歌曲，需要返回新的播放列表

   return QString(args);
}

void fm::on_channelButton_clicked(){
    channel.show();
//    channel.GetChannels();
//    emit toGetchannel();


}

void fm::on_deleteButton_clicked(){
    ui->playButton->setStyleSheet("QPushButton{border-image: url(:/image/pause.png);"
                                  "border:0px 0px 0px 0px;}"
                                  "QPushButton:pressed{border-image: url(:/image/pause-.png);}");
    b_PlayStatus = true;
    _TYPE = "b";
    GetSongs();
}

//void fm::on_rightButton_clicked()
//{
//    bool ok;
//    quint8 temp;
//    temp = _NOW_CHANNEL.toInt(&ok,10) + 1;
//    if(temp == 43)  temp = 0;
//    _NOW_CHANNEL = QString::number(temp,10);
//    DoubanChannels[array_at]
//    GetSongs();
//}

//void fm::on_leftButton_clicked()
//{
////    bool ok;
////    qint32 temp;
////    temp = _NOW_CHANNEL.toInt(&ok,10) - 1;
////    _NOW_CHANNEL = QString::number(temp,10);
////    if(temp == 0)   temp = 42;
////    _NOW_CHANNEL =  DoubanChannels[39];
////    qDebug()<<"Channel change to:"_NOW_CHANNEL;

//    GetSongs();
//}

void fm::on_voiceButton_clicked(){
    if(b_voiceON == true){
    ui->voiceButton->setStyleSheet("QPushButton{border-image: url(:/image/voiceoff.png);"
                                  "border:0px 0px 0px 0px;}"
                                  "QPushButton:pressed{border-image: url(:/image/voiceoff-.png);}");
    ui->volumeSlider->setValue(0);
    player->setVolume(0);
    b_voiceON = false;
    }else{
        ui->voiceButton->setStyleSheet("QPushButton{border-image: url(:/image/voice.png);"
                                      "border:0px 0px 0px 0px;}"
                                      "QPushButton:pressed{border-image: url(:/image/voice-.png);}");
        ui->volumeSlider->setValue(100);
        player->setVolume(100);
        b_voiceON = true;
    }
}
