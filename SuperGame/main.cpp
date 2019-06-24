#include "SuperMan.h"
#include <QApplication>
#include <QIcon>
#include "base/UI/uimeterprocessbar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UiMeterProcessBar bar;
    bar.setMaxValue(100);
    bar.setMinValue(-300);
    bar.setDuration(30);
    bar.show();


    SuperMan man;

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/img/wang.jpg")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);

    return a.exec();
}
