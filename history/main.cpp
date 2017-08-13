#include "fm.h"
#include <QApplication>
#include "custom.h"
#include "channel.h"
QString _usericon;

QString _USER_ID,
        _USER_ICON,
        _USER_TOKEN,
        _USER_EXPIRE,
        _SID,
        _TYPE,
        _NOW_CHANNEL;
bool    b_HasLogin;
//qint32 array_at;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    fm x;
    x.show();

//    custom y;
//    y.show();

//    channel z;
//    z.show();
    return a.exec();
}
