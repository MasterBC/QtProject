#ifndef SUPERMAN_H
#define SUPERMAN_H

#include <QObject>
QT_BEGIN_NAMESPACE
class LoginUI;
class LoadingUI;
class MainWindow;
class HomeUI;
class GameFrameUI;
class GameMap;
class HandleMsg;
class BaccaratUI;
class MahjongUI;
class LandlordUI;
class CowcowUI;
class ChineseChessUI;

enum class EnScene{
    Login = 0,
    Loading,
    Home,
    GameFrame,
    GameMap,
    NoScene = 1024
};
class SuperMan : public QObject
{
    Q_OBJECT
public:
    explicit SuperMan(QObject *parent = nullptr);
    ~SuperMan();

    void setSceneBGM(const QString& strMusic);                        // 设置场景音乐
    void closeAllScene();
signals:

public slots:
    void onChangeScene(EnScene curScene,EnScene toScene);       // 场景切换
    void onHandleMsg(QByteArray data);                          // 消息处理
    void onExitGame(int gameID);

protected:
    bool login();
    bool gameList();
    GameMap* currentMap();             // 进入游戏的哪种场景
private:
    // 内部接口 指令解析
    bool switchCMD(int mainID, int subID);
    bool handleRegister(int subID);
    bool handleLogin(int subID);            // 登录
    bool handleScene(int subID);            // 场景
    bool handleFrame(int subID);            // 框架
    bool handleState(int subID);            // 状态
    bool handlePlayerState(int subID);      // 玩家状态
    bool handleUpdate(int subID);           // 信息更新


    // 当前场景
    EnScene m_curScene;

    // 界面展示
    LoginUI*        m_login;
    LoadingUI*      m_loadingUI;
    HomeUI*         m_homeUI;
    GameFrameUI*    m_gameFrameUI;



    // 子游戏界面
    GameMap*        m_gameMapFather;
    BaccaratUI*     m_gameMapBaccarat;
    MahjongUI*      m_gameMapMahjong;
    LandlordUI*     m_gameMapLandlord;
    CowcowUI*       m_gameMapCowcow;
    ChineseChessUI* m_gameMapChineseChess;

    // 消息处理
    HandleMsg*      m_msg;
    std::string     m_data;

    // 信息更新
    int             m_updateCode; //更新所需子码
    std::string     m_updateInfo;//缓存信息
};
QT_END_NAMESPACE
#endif // SUPERMAN_H
