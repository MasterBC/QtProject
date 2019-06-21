#include "SuperMan.h"
#include <QApplication>


#include "base/UI/poker.h"

#include <QDebug>

#include <QIcon>

#include <QBitmap>
#include <QLabel>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SuperMan man;

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/img/wang.jpg")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);

    return a.exec();
}
