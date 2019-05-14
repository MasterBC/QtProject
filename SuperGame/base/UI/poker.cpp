#include "poker.h"
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>

#define LOGIC_MASK_COLOR 0xF0 //花色掩码
#define  LOGIC_MASK_VALUE 0x0F //数值掩码

Poker::Poker(QWidget *parent,bool haveCenter) : QWidget(parent),m_haveCenter(haveCenter)
{
    m_value = new QLabel(this);
    m_color = new QLabel(this);
    m_center = nullptr;

    QGridLayout* layout= new QGridLayout;
    layout->addWidget(m_value,0,0,1,1,Qt::AlignLeft);
    layout->addWidget(m_color,1,0,1,1,Qt::AlignLeft);
    if(m_haveCenter)
    {
        m_center = new QLabel(this);
        layout->addWidget(m_center,2,1,3,3,Qt::AlignLeft);
    }


    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setLayout(layout);
    setStyleSheet(QString("border-image: url(:/img/poker_b/b_bg.png);"));
    setAutoFillBackground(true);
}

void Poker::setColor(Poker::EnColor color)
{
    QPixmap pixmap;
    switch (color) {
    case EnColor::FangKuai:
        pixmap.load(":/img/poker_b/b_bigtag_0.png");
        break;
    case EnColor::MeiHua:
        pixmap.load(":/img/poker_b/b_bigtag_1.png");
        break;
    case EnColor::HongTao:
        pixmap.load(":/img/poker_b/b_bigtag_2.png");
        break;
    case EnColor::HeiTao:
        pixmap.load(":/img/poker_b/b_bigtag_3.png");
        break;
    case EnColor::UnKnown:
        pixmap.load(":/img/poker_b/b_bg.png");
        break;
    default:
        break;
    }

    if(m_haveCenter)
    {
        m_center->setPixmap(pixmap);
        m_center->setScaledContents(true);
    }

    m_color->setPixmap(pixmap);
    m_color->setScaledContents(true);
}

#include <QDebug>
void Poker::setCard(int card)
{
    int color = card & LOGIC_MASK_COLOR;
    int value = card & LOGIC_MASK_VALUE;
    if(card < 0x01 || 0x3D < card)
    {
        color = 0x40;
        value = 0x10;
    }

    QString strValue = "";
    EnColor enColor;
    switch (color) {
    case 0x00:
        strValue = ":/img/poker_b/b_orange_%1.png";
        enColor = EnColor::FangKuai;
        break;
    case 0x10:
        strValue = ":/img/poker_b/b_black_%1.png";
        enColor = EnColor::MeiHua;
        break;
    case 0x20:
        strValue = ":/img/poker_b/b_red_%1.png";
        enColor = EnColor::HongTao;
        break;
    case 0x30:
        strValue = ":/img/poker_b/b_black_%1.png";
        enColor = EnColor::HeiTao;
        break;
    default:
        enColor = EnColor::UnKnown;
        break;
    }

    setColor(enColor);
    if(0< value && value < 0x0E)
    {
        m_value->setPixmap(QPixmap(strValue.arg( (value & LOGIC_MASK_VALUE)-1)));
    }
    else if(value == 0x0E)
    {
        m_value->setPixmap(QPixmap(":/img/poker_b/b_smalltag_4.png"));
    }
    else if(value == 0x0F)
    {
        m_value->setPixmap(QPixmap(":/img/poker_b/b_smalltag_5.png"));
    }
    else
    {
         m_value->setPixmap(QPixmap(":/img/poker_b/b_bg.png"));
    }
    m_value->setScaledContents(true);
}

