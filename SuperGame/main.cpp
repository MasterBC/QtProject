#include "SuperMan.h"
#include <QApplication>
#include "login.pb.h"
#include <QDebug>

#include <QIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SuperMan man;
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/img/wang.jpg")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);
    return a.exec();
}
