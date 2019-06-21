#include "baccaratui.h"
#include "ui_baccaratui.h"
#include "base/global.h"
#include "network/handlemsg.h"
#include "games/baccarat/baccaratlogic.h"
#include "base/UI/poker.h"


// proto
#include <baccarat.pb.h>
#include <gamecomm.pb.h>

// Qt
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QEnterEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QStandardItem>

BaccaratUI::BaccaratUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::BaccaratUI)
{
    ui->setupUi(this);

    m_curState = 0;
    m_lastTime = 0;

    m_freeTime = 0;
    m_betTime = 0;
    m_openTime = 0;
    m_PlayerGold = 0;

    m_animatIsFinish = true;
    m_timer = new QTimer;
    m_select = nullptr;
    m_animationGroup = new QParallelAnimationGroup;
    m_cardGroup = new QSequentialAnimationGroup;
    m_modelPlayers = new QStandardItemModel(ui->listView);

    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView->setModel(m_modelPlayers);


    //六张扑克牌
    ui->poker_1->hide();
    ui->poker_2->hide();
    ui->poker_3->hide();
    ui->poker_4->hide();
    ui->poker_5->hide();
    ui->poker_6->hide();

    m_logic = new baccaratLogic();

    ui->label_Clip1->installEventFilter(this);
    ui->label_Clip2->installEventFilter(this);
    ui->label_Clip3->installEventFilter(this);
    ui->label_Clip4->installEventFilter(this);
    ui->label_Clip5->installEventFilter(this);

    ui->label_Zhuang->installEventFilter(this);
    ui->label_Ping->installEventFilter(this);//
    ui->label_Xian->installEventFilter(this);
    ui->label_ZDZ->installEventFilter(this);//庄对子
    ui->label_TDP->installEventFilter(this);//同点平
    ui->label_XDZ->installEventFilter(this);
    ui->label_XTW->installEventFilter(this);
    ui->label_ZTW->installEventFilter(this);


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
        strStateName = "开始-";
        m_lastTime = m_freeTime;
        m_select = nullptr;

        ui->label_GameInfo->setText("玩家游戏信息");
        ui->label_banker->setText(tr("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">庄</span></p></body></html>"));
        ui->label_player->setText(tr("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#5500ff;\">闲</span></p></body></html>"));

        ui->label_scrollText->setWords( "请准备,即将进入下注阶段" );
        break;
    case SubGameStatePlaying:
        strStateName = "下注-";
        m_lastTime = m_betTime;

        ui->label_scrollText->setWords( "开始下注啦!" );
        break;
    case SubGameStateOver:
        strStateName = "结束-";
        m_lastTime = m_openTime;

        //恢复下注按钮
        ui->label_Clip1->setStyleSheet(" background-color: rgb(255, 85, 255);");
        ui->label_Clip2->setStyleSheet(" background-color: rgb(85, 170, 255);");
        ui->label_Clip3->setStyleSheet(" background-color: rgb(255, 110, 53);");
        ui->label_Clip4->setStyleSheet(" background-color: rgb(255, 180, 157);");
        ui->label_Clip5->setStyleSheet(" background-color: rgb(0, 170, 127);");

        break;
    default:
        break;
    }

    m_timer->stop();
    if(0 < m_lastTime) m_timer->start( 1000 );
    ui->label_State->setText(tr("状态:") + strStateName);
    qDebug()<<QTime::currentTime()<<"百家乐的当前状态:"<<state<<strStateName;
}

void BaccaratUI::enterScene(int scene,const std::string &data)
{
    QString strSceneName = "";
    ui->label_scrollText->setWords(tr("欢迎来到 百家乐!"));
    go::GameBaccaratEnter enter;
    if(enter.IsInitialized() && enter.ParseFromArray(data.c_str(), data.length()))
    {
        // 各个阶段时长
        m_freeTime = enter.freetime() - 1;
        m_betTime = enter.bettime() - 1;
        m_openTime = enter.opentime() - 1;

        qint64 curtime = QDateTime::currentDateTime().toSecsSinceEpoch();//.toMSecsSinceEpoch();
        m_curState = scene;
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

        // 个人信息
        const go::PlayerInfo& info = enter.userinfo();
        ui->label_name->setText(tr("名字:")+info.name().c_str());
        m_PlayerGold = info.gold();
        //显示金币的时候,需除以100
        ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold/100,'f',2));


        //筹码
        if(4 < enter.chips_size())
        {
            ui->label_Clip1->setText( QString::number(enter.chips(0)) );
            ui->label_Clip2->setText( QString::number(enter.chips(1)) );
            ui->label_Clip3->setText( QString::number(enter.chips(2)) );
            ui->label_Clip4->setText( QString::number(enter.chips(3)) );
            ui->label_Clip5->setText( QString::number(enter.chips(4)) );
        }



        if(0 < m_lastTime) m_timer->start( 1000 );
        ui->label_State->setText(tr("状态:")+strSceneName);
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
    {
        go::GameBet bet;
        if(bet.IsInitialized() && bet.ParseFromArray(data.c_str(),data.length()))
        {
            qDebug()<<"其他玩家 下注区域:"<<bet.betarea()<<" 下注分数:"<<bet.betscore()/100;
        }

    }
        strFrameName = "下注";
        break;
    case SubGameFrameBetResult:
    {
        go::GameBetResult betResult;
        if(betResult.IsInitialized() && betResult.ParseFromArray(data.c_str(),data.length()))
        {
            qDebug()<<"下注结果 状态:"<<betResult.state()<<" 提示:"<<betResult.hints().c_str() ;
            if(0 == betResult.state())
            {

                m_PlayerGold -= betResult.betscore();
                ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold/100,'f',2));
                QString strInfo = tr("\n区域:%1 下注:%2").arg( m_logic->GetAreaText( betResult.betarea()).c_str() ).arg( betResult.betscore()/100);
                ui->label_GameInfo->setText(strInfo);
            }
        }
    }
        strFrameName = "下注结果";
        break;
    case SubGameFrameOver:
    {
        go::GameBaccaratOver over;
        if(over.IsInitialized() && over.ParseFromArray(data.c_str(),data.length()))
        {
            qDebug()<<"成功解析";
        }
        std::string strAwardarea = over.awardarea();
        std::string strPlayercard = over.playercard();
        std::string strBankercard = over.bankercard();
        // 点数比较
        int playCount = 0, bankCount = 0;
        m_logic->CompareCard(strPlayercard.c_str(), strBankercard.c_str(), playCount, bankCount);

        //显示点数
        ui->label_banker->setText(tr("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">庄:%1点</span></p></body></html>").arg(bankCount));
        ui->label_player->setText(tr("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#5500ff;;\">闲:%1点</span></p></body></html>").arg(playCount));

        // 发牌动画
        dealCardAnimation(strPlayercard,strBankercard);

        // 中奖区域动画
        awardAnimation(strAwardarea);
    }
        strFrameName = "结束";
        break;
    case SubGameFrameCheckout:
    {
        m_curState = SubGameStateOver;
        go::GameBaccaratOver checkOut;
        if(checkOut.IsInitialized() && checkOut.ParseFromArray(data.c_str(),data.length()))
        {
            qDebug()<<"成功解析";
        }

        checkOut.acquire();//玩家得分
        // 金币刷新
        m_lastTime = m_openTime;
        m_PlayerGold += checkOut.acquire();
        ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold/100,'f',2));

        // 显示中奖信息
        ui->label_GameInfo->setText( tr("\n\n本轮结束,赢了:%2金币\n").arg(checkOut.acquire()/100));
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
    
}

bool BaccaratUI::updateInfo(int code, const std::string &data)
{
    switch (code) {
    case GameUpdatePlayerList:
    {
        go::UserList userList;
        if(userList.IsInitialized() && userList.ParseFromArray(data.c_str(),data.length()))
        {
            qDebug()<<"更新玩家信息 成功解析";
            m_modelPlayers->clear();
            for(int i = 0; i < userList.allinfos_size(); i++)
            {// 玩家列表
                const  go::PlayerInfo& info = userList.allinfos(i);
                QStandardItem* item = new QStandardItem(info.name().c_str());
                m_modelPlayers->appendRow(item);
            }
        }
        return true;
    }
        break;
    default:
        break;
    }
    return false;
}

bool BaccaratUI::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == event->MouseButtonDblClick ) return true;

    if(m_curState == SubGameStatePlaying)
    {
        QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);

        if(mouseEvent->type() == mouseEvent->MouseButtonPress ){

            if(watched==ui->label_Clip1 || watched==ui->label_Clip2 || watched==ui->label_Clip3 || watched==ui->label_Clip4 || watched==ui->label_Clip5 )
            {
                ui->label_Clip1->setStyleSheet(" background-color: rgb(255, 85, 255);");
                ui->label_Clip2->setStyleSheet(" background-color: rgb(85, 170, 255);");
                ui->label_Clip3->setStyleSheet(" background-color: rgb(255, 110, 53);");
                ui->label_Clip4->setStyleSheet(" background-color: rgb(255, 180, 157);");
                ui->label_Clip5->setStyleSheet(" background-color: rgb(0, 170, 127);");

                m_select = qobject_cast<QLabel*>(watched);
                m_select->setStyleSheet("background-color:red");
            }
            else
            {
                if(m_select)
                {
                    QLabel* label = qobject_cast<QLabel*>(watched);
                    unsigned char area = m_logic->GetArea(label->text().toStdString());
                    if(0xFF != area)
                    {
                        // 发送下注信息
                        HandleMsg::GetInstance()->ReqPlayBet(area, m_select->text().toInt(nullptr,10)*100 );
                    }else
                    {
                        qDebug()<<"数据异常..."<<label->text().toStdString().c_str();
                    }


                }
            }
        }
    }





    return QWidget::eventFilter(watched, event);


}

void BaccaratUI::onTimeout()
{
    // 不开奖时隐藏扑克
    if(m_curState != SubGameStateOver)
    {
        ui->poker_1->hide();
        ui->poker_2->hide();
        ui->poker_3->hide();
        ui->poker_4->hide();
        ui->poker_5->hide();
        ui->poker_6->hide();
    }

    static bool bBlick = true;
    if (m_curState == SubGameStatePlaying && 0 < m_lastTime)
    {// 下注状态 文本进行闪烁提示

        if(bBlick)
        {
            ui->label_State->setStyleSheet("font-size:8pt;font-weight:200; color:#ff0000;");
        }
        else
        {
            ui->label_State->setStyleSheet("font-size:10pt;font-weight:400; color:#ff0000;");
        }
        bBlick = !bBlick;
    }
    else
    {
        ui->label_State->setStyleSheet("");
    }


    // 倒计时显示
    if(0 < m_lastTime && m_lastTime < 10)
    {
        QString strPicture = QString(":/img/number/%1.png").arg(m_lastTime);
        ui->label_Number->setPixmap(QPixmap(strPicture));
        m_lastTime--;
    }else if(10 <= m_lastTime)
    {
        m_lastTime--;
    }
    else{
        ui->label_Number->setPixmap(QPixmap(":/img/number/0.png"));
    }


}

void BaccaratUI::awardAnimation(const std::string &award)
{

    m_animationGroup->clear();
    QString strInfo = tr("\n开奖区域:");
    for(size_t i = 0; i < award.length(); i++)
    {
        unsigned char card = award.at(i);
        if(1 == card)
        {
            strInfo +=tr(" ")+ m_logic->GetAreaText(i).c_str();
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

                QPropertyAnimation *animation = new QPropertyAnimation(pArea,"geometry");
                animation->setDuration(1000);

                QRect rect = pArea->geometry();
                animation->setStartValue(QRect(rect.x() +rect.width()/2, rect.y() + rect.height()/2, 0, 0));
                animation->setEndValue(rect);
                animation->setLoopCount(6);
                //CosineCurve OutBounce OutElastic OutInElastic OutInBounce InElastic OutInCirc  InOutBounce
                animation->setEasingCurve(QEasingCurve::OutInBounce);
                m_animationGroup->addAnimation(animation);

                qDebug()<<"有动画";
            }

        }
    }

    ui->label_scrollText->setWords( strInfo );
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




