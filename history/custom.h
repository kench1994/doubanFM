#ifndef CUSTOM_H
#define CUSTOM_H


#include <QDialog>
#include "doubanfm.h"
#include <QOBJECT>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QtNetwork>
#include <QDebug>
#include <QCloseEvent>

namespace Ui {
class custom;
}

class custom : public QDialog
{
    Q_OBJECT

public:
    explicit custom(QWidget *parent = 0);
    ~custom();
//    QString location;	//用户的常居地	//用户可能没有常居地
//    QString uid	;       //用户在豆瓣上的个人域名
//    QString name;       //用户的名号
//    QString content;	//用户的自我介绍
//    QString rel;        //用户的豆瓣页面
//    QString icon;       //用户的头像图片链接
//    QString homepage;   //用户的主页
    void Login(const QString &email, const QString &password);
    void FindUserInfo(QString userInfo);
    void setUserIcon();
signals:
    void loginSucceed();
//    void userIconReady();
//    void loginFailed(const QString &errmsg);

private slots:
    void onReceivedAuth(QNetworkReply *reply);
    void userIconReply(QNetworkReply *reply);
    void userInfoReply(QNetworkReply *reply);

    void on_cancelButton_clicked();
    void on_loginButton_clicked();
    void LoginDeal();
    void test();
private:
    Ui::custom *ui;
    QPoint m_WindowPos;
    QPoint m_MousePos;
    bool m_MousePressed;
    DoubanFM& doubanfm;
    QNetworkAccessManager *userManager;
    QString GetDATA;

    QNetworkAccessManager *userIconGetter;
    QNetworkAccessManager *userInfoGetter;
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *lpEvent);
    void mouseReleaseEvent(QMouseEvent *lpEvent);

};

#endif // CUSTOM_H
