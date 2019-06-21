#ifndef LANDLORDUI_H
#define LANDLORDUI_H
#include "base/UI/gamemap.h"


namespace Ui {
class LandlordUI;

}
class QHBoxLayout;
class LandlordUI : public GameMap
{
    Q_OBJECT

public:
    explicit LandlordUI(QWidget *parent = 0);
    ~LandlordUI();

    void enterScene(int scene,const std::string& data);
    void changeState(int state,const std::string& data);
    bool gameHandle(int code,const std::string& data);
    bool updateInfo(int code, const std::string& data);
protected:
    void readyState();
    void enterState();
    void startState();
    void overState();

private slots:

    void on_pushButton_Hint_clicked();

    void on_pushButton_NoOut_clicked();

    void on_pushButton_Out_clicked();

private:
    void initUI();
    
    void resetTabel();
    void faPai();


    int m_curState;
    int m_userID;
    qint64 m_playerGold;
    QHBoxLayout*  m_layoutCards;
    Ui::LandlordUI *ui;
};

#endif // LANDLORDUI_H
