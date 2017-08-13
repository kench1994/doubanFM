#ifndef CHANNEL_H
#define CHANNEL_H

#include <QDialog>
#include <QOBJECT>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QtCore>
#include "douban_types.h"

namespace Ui {
class channel;
}

class channel : public QDialog
{
    Q_OBJECT

public:
    explicit channel(QWidget *parent = 0);
    ~channel();
    DoubanChannel DoubanChannels[39];

//    void GetChannels();
signals:
    void changeChannel();



private slots:
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();
//    void on_pushButton_3_clicked();
//    void on_pushButton_4_clicked();
//    void on_pushButton_5_clicked();
//    void on_pushButton_6_clicked();
//    void on_pushButton_7_clicked();
//    void on_pushButton_8_clicked();
//    void on_pushButton_9_clicked();
//    void on_pushButton_10_clicked();
//    void on_pushButton_11_clicked();
//    void on_pushButton_12_clicked();
//    void on_pushButton_13_clicked();
//    void on_pushButton_14_clicked();

    void on_cancelButton_clicked();
    void onReceiveChannelList(QNetworkReply *);


    void on_channelWidget_cellDoubleClicked(int row, int column);

private:
    Ui::channel *ui;
    QPoint m_WindowPos;
    QPoint m_MousePos;
    bool m_MousePressed;
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *lpEvent);
    void mouseReleaseEvent(QMouseEvent *lpEvent);
    QNetworkAccessManager *channelGetter;
};

#endif // CHANNEL_H
