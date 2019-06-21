#include "landlordui.h"
#include "ui_landlordui.h"
#include "network/handlemsg.h"
#include "base/global.h"
#include "base/UI/poker.h"


//proto
#include "landLords.pb.h"


//Qt
#include <QBitmap>
#include <QDebug>
#include <QHBoxLayout>

LandlordUI::LandlordUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::LandlordUI)
{
    ui->setupUi(this);
    m_userID = 0;
    m_curState = SubGameSenceFree;
    initUI();
}

LandlordUI::~LandlordUI()
{
    delete ui;
}

void LandlordUI::enterScene(int scene, const std::string &data)
{
    go::GameLandLordsEnter enter;
    if(enter.IsInitialized() && enter.ParseFromArray(data.c_str(), data.length()))
    {
        // qint64 curtime = QDateTime::currentDateTime().toSecsSinceEpoch();//.toMSecsSinceEpoch();
        m_curState = scene;
        QString strSceneName;
        switch (scene) {
        case SubGameSenceStart:
            strSceneName = "开始";
            this->readyState();
            break;
        case SubGameSencePlaying:
            strSceneName = "下注";
            this->startState();
            break;
        case SubGameSenceOver:
            strSceneName = "结算";
            this->overState();
            break;
        case SubGameSenceFree://当前无用(准备阶段)
            strSceneName = "空闲";
            this->enterState();
            break;
        default:
            break;
        }

        m_userID = enter.userid();
        const go::UserList& list = enter.players();
        for(int i = 0; i < list.allinfos_size(); i++)
        {// 玩家列表
            const  go::PlayerInfo& info = list.allinfos(i);
            if(info.userid() == m_userID)
            {// 个人信息
                ui->label_name->setText(tr("名字:")+info.name().c_str());
                m_playerGold = info.gold();

                //显示的时候,需除以100
                ui->label_gold->setText(tr("金币:")+QString::number(m_playerGold/100,'f',2));
            }
        }


    }

}
void LandlordUI::changeState(int state, const std::string &data)
{

    switch (state) {
    case SubGameStateStart:
    {

      resetTabel();
        go::GameLandLordsBegins beginInfo;
        if(beginInfo.IsInitialized() && beginInfo.ParseFromArray(data.c_str(), data.length()))
        {

            std::string strBottomCard = beginInfo.cardsbottom();
            std::string strHandCard = beginInfo.cardshand();

            //            for(int i = 0; i < strHandCard.length(); i++)
            //            {
            //                Poker* poker = new Poker;
            //                poker->setCard(strHandCard.at(i));
            //                m_layoutCards->addWidget(poker);
            //            }

            for(int i = 0; i < strHandCard.length(); i++)
            {
                Poker* poker = new Poker;
                poker->setCard(strHandCard.at(i));
                m_layoutCards->addWidget(poker);
            }


        }
    }
        this->startState();
        break;
    case SubGameStatePlaying:
        this->startState();
        break;
    case SubGameStateOver:
        this->overState();
        break;
    default:
        break;
    }
}


bool LandlordUI::gameHandle(int code, const std::string &data)
{

    switch (code) {
    case SubGameFrameReady:
    {

        go::GameReady ready;
        if(ready.IsInitialized() && ready.ParseFromArray(data.c_str(), data.length()))
        {
            if (ready.userid() == m_userID)
            {// 是否自己准备
                readyState();
                m_curState = SubGameFrameReady;
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

bool LandlordUI::updateInfo(int code, const std::string &data)
{
    return false;
}

void LandlordUI::readyState()
{
    ui->pushButton_Hint->setText("已准备");
    ui->pushButton_Hint->show();
    ui->pushButton_NoOut->hide();
    ui->pushButton_Out->hide();
}

void LandlordUI::enterState()
{
    ui->pushButton_Hint->setText("准备");
    ui->pushButton_Hint->show();
    ui->pushButton_NoOut->hide();
    ui->pushButton_Out->hide();
}

void LandlordUI::startState()
{
    ui->pushButton_Hint->setText("提示");
    ui->pushButton_Hint->show();
    ui->pushButton_NoOut->setText("不出");
    ui->pushButton_NoOut->show();
    ui->pushButton_Out->setText("出牌");
    ui->pushButton_Out->show();
}

void LandlordUI::overState()
{
    ui->pushButton_Hint->hide();
    ui->pushButton_NoOut->hide();
    ui->pushButton_Out->hide();
}




void LandlordUI::initUI()
{

    m_layoutCards = new QHBoxLayout(ui->groupBox_player1);
    //    QPixmap mypixmap;
    //    ui->pushButton->text().clear();
    //    mypixmap.load(":/img/landlord/item_chupai.png");
    //    ui->pushButton->setMask(mypixmap.mask());
    //    ui->pushButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    //    ui->pushButton->setIcon(mypixmap);
    //    ui->pushButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

}

void LandlordUI::resetTabel()
{
    enterState();
    QLayoutItem *child;
    while ((child = m_layoutCards->takeAt(0)) != 0) {
        child->widget()->deleteLater();
        delete child;
    }
}

void LandlordUI::on_pushButton_Hint_clicked()
{

    HandleMsg::GetInstance()->ReqReady(m_userID, true);
    if(m_curState == SubGameSenceFree)
    {//发送准备
        //S_GameKindID
        //HandleMsg::GetInstance()->ReqReady(m_userID, true);
    }else{//提示

    }
}

void LandlordUI::on_pushButton_NoOut_clicked()
{

}

void LandlordUI::on_pushButton_Out_clicked()
{

}
