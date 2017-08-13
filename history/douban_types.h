#ifndef DOUBAN_TYPES_H
#define DOUBAN_TYPES_H

#include <QtCore>


static const QString API_SONGLIST = "http://douban.fm/j/mine/playlist";
static const QString API_SONGLIST_LOG = "https://www.douban.com/j/app/radio/people";
static const QString FM_API_ADDR = "http://api.douban.com/people/";
static const QString FM_LOGIN = "https://www.douban.com/j/app/login";

struct DoubanUser {
//public:
    QString user_id;
    QString expire;
    QString token;
    QString user_name;
    QString email;
    QString password;
};

struct DoubanChannel {
    QString name;
    qint32 seq_id;
    QString abbr_en;
    QString channel_id;
    QString name_en;
//    qint32 array_at;//自己定义的,方便编程
};


struct DoubanFMSong {
    QString album;
    QString picture;
    QString ssid;
    QString artist;
    QString url;
    QString company;
    QString title;
    double rating_avg;
    quint32 length;
    QString subtype;
    QString public_time;
    quint32 sid;
    quint32 aid;
    QString albumtitle;
    bool like;
};


#endif // DOUBAN_TYPES_H
