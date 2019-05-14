////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): chronograph.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-04-26
/// 功能(@brief) ：    计时表
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////

#ifndef CHRONOGRAPH_H
#define CHRONOGRAPH_H

#include <QWidget>

class Chronograph : public QWidget
{
    Q_OBJECT
public:
    explicit Chronograph(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // CHRONOGRAPH_H
