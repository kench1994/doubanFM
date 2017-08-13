#include "channel.h"
#include "ui_channel.h"
#include <QDebug>
#include <QMouseEvent>
#include <QTableWidget>
#include <QTableWidgetItem>

extern QString _NOW_CHANNEL;

channel::channel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::channel)
{
    ui->setupUi(this);

    /*set Ui*/
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->channelWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    channelGetter = new QNetworkAccessManager(this);
    connect(channelGetter,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReceiveChannelList(QNetworkReply*)));
    channelGetter->get(QNetworkRequest(QUrl("http://www.douban.com/j/app/radio/channels")));


//    qDebug()<<"channel:"<<DoubanChannel.name_en;
}

channel::~channel()
{
    delete ui;
    delete channelGetter;

}


void channel::mousePressEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton){
        m_WindowPos = this->pos();
        m_MousePos = lpEvent->globalPos();
        this->m_MousePressed = true;
    }
}

void channel::mouseReleaseEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton)
    {
        this->m_MousePressed = false;
    }
}

void channel::mouseMoveEvent(QMouseEvent *lpEvent){
    if (m_MousePressed)
    {
        this->move(m_WindowPos + (lpEvent->globalPos() - m_MousePos));
    }
}

void channel::onReceiveChannelList(QNetworkReply *reply){
   if (reply->error() != QNetworkReply::NetworkError::NoError) {
        qWarning() << "Error occurred while receiving new list: " << reply->errorString();
        reply->deleteLater();
        return;
    }

   qDebug()<<"getting channels.";
    //为了能显示下载的网页中的中文，我们使用了QTextCodec 类对象，应用utf8编码
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString all = codec->toUnicode(reply->readAll());

    QList<DoubanChannel> channels;
    QJsonParseError parseerr;
    QVariant result = QJsonDocument::fromJson(all.toUtf8(), &parseerr).toVariant();

    if (parseerr.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        if (obj["r"].toInt() != 0) {
            qDebug() << Q_FUNC_INFO << "Err" << obj["err"].toString();
            reply->deleteLater();
            return;
        }
        uint i=0,j=1,c=1;
        DoubanChannels[0].channel_id = "-3";
        QVariantList channelList = obj["channels"].toList();
//        qDebug()<<channelList<<"\n";
//        qDebug()<<"------------------------------------------------------------------------\n";
        foreach(const QVariant& item, channelList) {
            QVariantMap ch = item.toMap();
//            qDebug()<<channel;

            DoubanChannels[c].name = ch["name"].toString();
            DoubanChannels[c].channel_id = QString::number(ch["channel_id"].toInt(),10);
//            DoubanChannels[c].array_at = 3*i+j;
//            DoubanChannels[c].name_en = ch["name_en"].toString();
//            DoubanChannels[c].seq_id = ch["seq_id"].toInt();
//            DoubanChannels[c].abbr_en = ch["abbr_en"].toString();
//            qDebug()<<"channel_id"<<DoubanChannels[c].channel_id;
//            qDebug()<<"name:"<<DoubanChannels[c].name;
//            qDebug()<<"name_en"<<DoubanChannels[c].name_en;
//            qDebug()<<"seq_id:"<<DoubanChannels[c].seq_id;
//            qDebug()<<"abbr_en"<<DoubanChannels[c].abbr_en;
            channels.append(DoubanChannels[c]);
            ui->channelWidget->setItem(i,j,new QTableWidgetItem(DoubanChannels[c].name));
            c++;
            if(j<2) j++;
            else{
                ui->channelWidget->insertRow(ui->channelWidget->rowCount());
                j=0;
                i++;   }



        }

    }
    reply->deleteLater();
}


//void channel::on_pushButton_clicked(){
//    _NOW_CHANNEL = "-3"; emit this->changeChannel();  qDebug()<<_NOW_CHANNEL;  this->close();}
//void channel::on_pushButton_2_clicked(){
//    _NOW_CHANNEL = "0"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_3_clicked(){
//    _NOW_CHANNEL = "1"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_4_clicked(){
//    _NOW_CHANNEL = "2"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_5_clicked(){
//    _NOW_CHANNEL = "3"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_6_clicked(){
//    _NOW_CHANNEL = "4"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_7_clicked(){
//    _NOW_CHANNEL = "5"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_8_clicked(){
//    _NOW_CHANNEL = "6"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_9_clicked(){
//    _NOW_CHANNEL = "7"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_10_clicked(){
//    _NOW_CHANNEL = "8"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_11_clicked(){
//    _NOW_CHANNEL = "9"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_12_clicked(){
//    _NOW_CHANNEL = "10"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_13_clicked(){
//    _NOW_CHANNEL = "11"; emit this->changeChannel(); this->close();}
//void channel::on_pushButton_14_clicked(){
//    _NOW_CHANNEL = "12"; emit this->changeChannel(); this->close();}


void channel::on_cancelButton_clicked()
{
    this->close();
}

void channel::on_channelWidget_cellDoubleClicked(int row, int column)
{
//    qDebug()<<"channel_id:"<<DoubanChannels[3*row+column].channel_id;
//    _NOW_CHANNEL = QString::number(DoubanChannels[3*row+column].channel_id,10);
    _NOW_CHANNEL = DoubanChannels[3*row+column].channel_id;
//    qDebug()<<DoubanChannels[3*row+column].array_at;
     emit this->changeChannel(); this->close();
}
