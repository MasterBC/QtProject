#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "base/UI/uibase.h"
class GameMap:public UIbase
{
    Q_OBJECT
public:
    using UIbase::UIbase;
    explicit GameMap(QWidget *parent = 0);
    ~GameMap();
    
    inline virtual int getID(){return m_id;}
    inline virtual void setID(int id){m_id = id;}
    
    //场景消息
    virtual void enterScene(int scene, const std::string& data) = 0;
    //框架消息(状态切换)
    virtual void changeState(int state,const std::string& data)=0;
    //游戏处理
    virtual bool gameHandle(int code, const std::string& data) = 0;
    //更新游戏信息
    virtual bool updateInfo(int code, const std::string& data)=0;

signals:
    void exitSig(int gameID);
    
protected:
    void closeEvent(QCloseEvent *event);
    
private:
    int m_id;
};

#endif // GAMEMAP_H
