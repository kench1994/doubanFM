#ifndef DOUBAN_H
#define DOUBAN_H

#include <QObject>
#include "douban_types.h"
#include <memory>


class DoubanFM : public QObject
{
    Q_OBJECT
public:
    explicit DoubanFM(QObject *parent = 0);
    ~DoubanFM();
    static DoubanFM &getInstance();
    void setUser(const DoubanUser& user);


    std::unique_ptr<DoubanUser> _user;
};



#endif // DOUBAN_H
