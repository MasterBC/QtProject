#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "base/UI/uibase.h"
class GameMap:public UIbase
{
    Q_OBJECT
public:
    explicit GameMap(QWidget *parent = 0);
    ~GameMap();

    inline virtual int getID(){return m_id;}
    inline virtual void setID(int id){m_id = id;}

    virtual void changeState(int state,const std::string& data)=0;
    virtual void enterScene(int scene, const std::string& data) = 0;
    virtual bool gameHandle(int code, const std::string& data) = 0;
signals:
    void exitSig(int gameID);

protected:
    void closeEvent(QCloseEvent *event);

private:
    int m_id;
};

#endif // GAMEMAP_H
