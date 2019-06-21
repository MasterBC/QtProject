#include "gamemap.h"
#include "network/handlemsg.h"
#include <QMessageBox>
#include <QDebug>
GameMap::GameMap(QWidget *parent):
    UIbase(parent),m_id(0)
{
    //如果服务端断开则退出游戏
    //connect(HandleMsg::GetInstance(),SIGNAL(disconnectSig()),this,SLOT(close()));
}

GameMap::~GameMap()
{

}

void GameMap::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    stopBGM();
    qDebug()<<"次数 --- >";
    emit exitSig(m_id);
}
