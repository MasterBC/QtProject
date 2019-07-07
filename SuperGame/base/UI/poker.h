////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): poker.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-05-05
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef POKER_H
#define POKER_H

#include <QLabel>
//class QLabel;
class Poker : public QLabel
{
    Q_OBJECT
public:
    enum class EnColor{
        FangKuai = 0,
        MeiHua,
        HongTao,
        HeiTao,
        UnKnown
    };
    Poker(QWidget *parent = nullptr,bool haveCenter = false);
    void setCard(int card);
    void setCenter(bool isCenter);
    void setBackground(bool isBack);
signals:
    
public slots:

protected:
    void setColor(EnColor color);
private:
    QLabel* m_value;
    QLabel* m_color;
    QLabel* m_center;
    bool m_haveCenter;
};

#endif // POKER_H
