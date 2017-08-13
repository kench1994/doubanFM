#ifndef FM_H
#define FM_H



#include <QWidget>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <QWheelEvent>
#include "song.h"
#include "doubanfm.h"
#include "custom.h"
#include "channel.h"

namespace Ui {
class fm;
class custom;
class channel;

}

//class DoubanFM;

class fm : public QWidget
{
    Q_OBJECT
signals:
    void closeLoginPanel();
    void receivedNewList(const QList<DoubanFMSong>& songs);

public:
    explicit fm(QWidget *parent = 0);
    ~fm();
    custom user;
    channel channel;
//    song sg_SongList;
    DoubanFMSong DoubanSong;
//    void FindSongsList(QString Songs);
    void GetSongs();
    void UpdateTime(qint64 time);
    void _Play();
    void getNewPlayList();
//    void getPlayingList();
    QString commitForm(QString type);


private slots:
    void scrollCaption();
    void on_playButton_clicked();
    void on_nextButton_clicked();
    void on_likeButton_clicked();
//    void on_volumeButton_clicked();
    void slidChannel();
    void on_userButton_clicked();
//    void songReply(QNetworkReply *reply);
    void imgReply(QNetworkReply *reply);
    QString CovertTime(qint64 ti);
    void durationChanged(qint64);
    void positionChanged(qint64 position);
    void setVolume(int position);
    void onReceivedNewList(QNetworkReply *reply);
    void on_channelButton_clicked();
    void on_deleteButton_clicked();
//    void on_rightButton_clicked();
//    void on_leftButton_clicked();
    void on_voiceButton_clicked();


private:
    Ui::fm *ui;
    QPoint qp_WindowPos;
    QPoint qp_MousePos;
    bool b_MousePressed;
    bool b_PlayStatus;
    bool b_voiceON;
//    bool b_ClickedVol;
//    unsigned int ui_NowPlay;
    QString GetDATA;
    QTimer *m_timer;
    qint64  allduration;
    QUrl songUrl;
    QString m_scrollCaptionStr ;
//    QNetworkAccessManager *songManager;
    QNetworkAccessManager *albumImg;
    QNetworkAccessManager *songDeal;


    //after login
    QNetworkAccessManager *newList;
    QNetworkAccessManager *playingList;



    QMediaPlayer* player;
    /*struct*/

//    DoubanFMSong Song;
    //event
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *lpEvent);
    void mouseReleaseEvent(QMouseEvent *lpEvent);
};

#endif // FM_H
