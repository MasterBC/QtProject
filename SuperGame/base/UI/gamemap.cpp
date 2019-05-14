#include "gamemap.h"


GameMap::GameMap(QWidget *parent):
    UIbase(parent),m_id(0)
{

}

GameMap::~GameMap()
{

}

void GameMap::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    stopBGM();
    emit exitSig(m_id);
}
