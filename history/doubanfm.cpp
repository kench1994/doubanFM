#include "doubanfm.h"



DoubanFM::DoubanFM(QObject *parent) : QObject(parent) {
}

DoubanFM::~DoubanFM() {
    this->disconnect();
}

DoubanFM& DoubanFM::getInstance() {
    static DoubanFM _INSTANCE(nullptr);
    return _INSTANCE;
}


