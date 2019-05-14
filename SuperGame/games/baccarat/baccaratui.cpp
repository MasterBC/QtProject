#include "baccaratui.h"
#include "ui_baccaratui.h"
#include "base/global.h"
#include "network/handlemsg.h"
#include "games/baccarat/baccaratgamelogic.h"
#include "base/UI/poker.h"


// proto
#include <baccarat.pb.h>

// Qt
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

float betMoney = 400.0;
BaccaratUI::BaccaratUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::BaccaratUI)
{
    m_curState = 0;
    m_lastTime = 0;

    m_freeTime = 0;
    m_betTime = 0;
    m_openTime = 0;
    m_PlayerGold = 0.0;

    m_animatIsFinish = true;
    m_timer = new QTimer;
    m_animationGroup = new QParallelAnimationGroup;
    m_cardGroup = new QSequentialAnimationGroup;
    ui->setupUi(this);


    //六张扑克牌
    ui->poker_1->hide();
    ui->poker_2->hide();
    ui->poker_3->hide();
    ui->poker_4->hide();
    ui->poker_5->hide();
    ui->poker_6->hide();

    m_logic = new baccaratGameLogic();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

}

BaccaratUI::~BaccaratUI()
{
    delete ui;
    if(m_timer)
    {
        m_timer->deleteLater();
    }
    if(m_animationGroup)
    {
        m_animationGroup->deleteLater();
    }
    if(m_cardGroup)
    {
        m_cardGroup->deleteLater();
    }
}



void BaccaratUI::changeState(int state,const std::string &data)
{
    
    Q_UNUSED(data)
    m_curState = state;
    QString strStateName = "";

    switch (state) {
    case SubGameStateStart:
        strStateName = "开始";
        m_lastTime = m_freeTime;
        break;
    case SubGameStatePlaying:
        HandleMsg::GetInstance()->ReqPlayBet(2,betMoney);
        strStateName = "下注";
        m_lastTime = m_betTime;
        break;
    case SubGameStateOver:
        strStateName = "结束";
        m_lastTime = m_openTime;
        break;
    default:
        break;
    }
    m_timer->stop();
    if(0 < m_lastTime) m_timer->start( 1000 );
    ui->label_State->setText(tr("当前状态:")+strStateName);

    qDebug()<<QTime::currentTime()<<"百家乐的当前状态:"<<state<<strStateName;
}

void BaccaratUI::enterScene(int scene,const std::string &data)
{
    QString strSceneName = "";
    
    go::GameBaccaratEnter enter;
    if(enter.IsInitialized() && enter.ParseFromArray(data.c_str(), data.length()))
    {
        m_freeTime = enter.freetime() - 1;
        m_betTime = enter.bettime() - 1;
        m_openTime = enter.opentime() - 1;


        qint64 curtime = QDateTime::currentDateTime().toSecsSinceEpoch();//.toMSecsSinceEpoch();
        switch (scene) {
        case SubGameSenceStart:
            strSceneName = "开始";
            m_lastTime = m_freeTime;
            break;
        case SubGameSencePlaying:
            strSceneName = "下注";
            m_lastTime = m_betTime;
            break;
        case SubGameSenceOver:
            strSceneName = "结算";
            m_lastTime = m_openTime;
            break;
        case SubGameSenceFree://当前无用
            strSceneName = "空闲";
            break;
        default:
            break;
            
        }
        m_lastTime = m_lastTime -(curtime- enter.timestamp());
        const go::UserList& list = enter.players();
        for(int i = 0; i < list.allinfos_size(); i++)
        {
            const  go::PlayerInfo& info = list.allinfos(i);
            if(info.userid() == enter.userid())
            {
                ui->label_name->setText(tr("名字:")+info.name().c_str());
                m_PlayerGold = info.gold();
                ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold,'f',2));
            }
        }

        if(0 < m_lastTime) m_timer->start( 1000 );
        ui->label_State->setText(tr("当前状态:")+strSceneName);
        qDebug()<<QTime::currentTime()<<enter.timestamp()<<"百家乐的当前场景:"<<scene<<curtime<<strSceneName<< m_lastTime;
    }
    
    

}

bool BaccaratUI::gameHandle(int code,const std::string &data)
{
    
    QString strFrameName = "";
    
    switch (code) {
    case SubGameFrameStart:
        strFrameName = "开始";
        break;
    case SubGameFramePlaying:
        strFrameName = "下注";
        break;
    case SubGameFrameBetResult:
        strFrameName = "下注结果";
        ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold - betMoney,'f',2));
        break;
    case SubGameFrameOver:
    case SubGameFrameCheckout:
    {
        go::GameBaccaratOver checkOut;
        if(checkOut.IsInitialized() && checkOut.ParseFromArray(data.c_str(),data.length()))
        {
            qDebug()<<"成功解析";
        }
        checkOut.acquire();//玩家得分
        std::string strAwardarea = checkOut.awardarea();
        std::string strPlayercard = checkOut.playercard();
        std::string strBankercard = checkOut.bankercard();

        // 发牌动画
        dealCardAnimation(strPlayercard,strBankercard);

        // 中奖区域动画
        awardAnimation(strAwardarea);

        // 金币刷新
        m_lastTime = m_openTime;
        m_PlayerGold += checkOut.acquire();
        ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold,'f',2));

        int playCount = 0, bankCount = 0;
        m_logic->CompareCard(strPlayercard.c_str(),strBankercard.c_str(),playCount,bankCount);
        ui->label_State->setText(tr("当前状态:结算"));

        m_curState = SubGameStateOver;
    }
        strFrameName = "结算结果";
        break;
    case SubGameFrameSetHost:
        strFrameName = "定庄";
        break;
    case SubGameFrameHost:
        strFrameName = "抢庄";
        break;
    case SubGameFrameSuperHost:
        strFrameName = "超级抢庄";
        break;
    case SubGameFrameResult:
        strFrameName = "结果信息";
        break;
    default:
        break;
    }
    
    qDebug()<<QTime::currentTime()<<"百家乐 服务端反馈:"<<code<<strFrameName;
    return true; //临时
    
    return false;
}

void BaccaratUI::onTimeout()
{
    if(m_curState != SubGameStateOver)
    {
        ui->poker_1->hide();
        ui->poker_2->hide();
        ui->poker_3->hide();
        ui->poker_4->hide();
        ui->poker_5->hide();
        ui->poker_6->hide();
    }

    if(0 < m_lastTime && m_lastTime < 10)
    {
        QString strPicture = QString(":/img/number/%1.png").arg(m_lastTime);
        ui->label_Number->setPixmap(QPixmap(strPicture));
        m_lastTime--;
    }
    else{
        ui->label_Number->setPixmap(QPixmap(":/img/number/0.png"));
    }


}

void BaccaratUI::awardAnimation(const std::string &award)
{

    m_animationGroup->clear();
    for(size_t i = 0; i < award.length(); i++)
    {
        unsigned char card = award.at(i);
        if(1 == card)
        {

            QLabel *pArea = nullptr;
            switch (i) {
            case AREA_XIAN:
                pArea = ui->label_Xian;
                break;
            case AREA_PING:
                pArea = ui->label_Ping;
                break;
            case AREA_ZHUANG:
                pArea = ui->label_Zhuang;
                break;
            case AREA_XIAN_TIAN:
                pArea = ui->label_XTW;
                break;
            case AREA_ZHUANG_TIAN:
                pArea = ui->label_ZTW;
                break;
            case AREA_TONG_DUI:
                pArea = ui->label_TDP;
                break;
            case AREA_XIAN_DUI:
                pArea = ui->label_XDZ;
                break;
            case AREA_ZHUANG_DUI:
                pArea = ui->label_ZDZ;
                break;
            default:
                qDebug()<<"无效区域";
                break;
            }

            if(nullptr != pArea)
            {
                QPropertyAnimation *animation = new QPropertyAnimation(pArea,"size");
                animation->setDuration(2000);

                QSize rect = pArea->size();
                animation->setStartValue(QSize(rect.width()/4,rect.height()/4));
                animation->setEndValue(rect);

                animation->setLoopCount(3);
                //CosineCurve OutBounce
                animation->setEasingCurve(QEasingCurve::OutBounce);
                m_animationGroup->addAnimation(animation);

                qDebug()<<"有动画";
            }

        }
    }
    m_animationGroup->start();
}

void BaccaratUI::dealCardAnimation(const std::string &strPlayerCard, const std::string &strBankerCard)
{
    if(!m_animatIsFinish)return;

    if(m_cardGroup)m_cardGroup = new QSequentialAnimationGroup;

    // 设置张牌
    qDebug()<<"子集合个数:";
    ui->poker_1->show();
    ui->poker_2->show();
    ui->poker_3->show();
    ui->poker_4->show();
    ui->poker_5->show();
    ui->poker_6->show();
    ui->poker_1->setCard(strBankerCard.at(0));
    ui->poker_2->setCard(strBankerCard.at(1));
    ui->poker_3->setCard(strBankerCard.at(2));
    ui->poker_4->setCard(strPlayerCard.at(0));
    ui->poker_5->setCard(strPlayerCard.at(1));
    ui->poker_6->setCard(strPlayerCard.at(2));


}




