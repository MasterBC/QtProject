#include "handlemsg.h"

// local
#include "base/global.h"
#include "base/common.h"

// proto
#include "comand.pb.h"
#include "gamecomm.pb.h"
#include "login.pb.h"
#include "baccarat.pb.h"
#include "mahjong.pb.h"


// Qt
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>

using namespace NSNetwork;

HandleMsg::HandleMsg(QObject *parent):
    QObject(parent)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    m_theme = 0;
    m_state = 0;
    m_socket = nullptr;
    registerProtoMsg();                // -------------> 注册消息
    m_socket = new QTcpSocket(this);
    connect(m_socket,SIGNAL(connected()),this,SLOT(SlotSend()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(SlotRead())); //接收发来的数据
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
}

HandleMsg::~HandleMsg()
{
    if(nullptr != m_socket)
    {
        m_socket->close();
        delete m_socket;
    }
    m_socket = nullptr;
}

HandleMsg* HandleMsg::GetInstance()
{
    std::shared_ptr<HandleMsg> msg = Magic_Singleton<HandleMsg>::GetInstance(nullptr);
    return msg.get();
}

bool HandleMsg::connectTo(QString &strIP, int nPort, int msecs)
{

    m_socket->disconnectFromHost();
    m_socket->connectToHost(strIP,nPort);
    while( m_socket->state()&&QAbstractSocket::ConnectedState)
    {
        qDebug()<<"成功连接->"<<strIP<<" "<<nPort;
        return m_socket->waitForConnected(msecs);
    }
    qDebug()<<"连接失败->"<<strIP<<" "<<nPort;
    return false;
}

bool HandleMsg::localHost(int nPort)
{
    m_socket->disconnectFromHost();
    m_socket->connectToHost(QHostAddress::LocalHost,nPort);
    while( m_socket->state()&&QAbstractSocket::ConnectedState)
    {
        qDebug()<<"成功连接本地->"<<nPort;
        return m_socket->waitForConnected();
    }
    qDebug()<<"本地连接失败->"<<nPort;
    return false;
}

void HandleMsg::sendIMMsg(char *data, size_t size)
{
    m_socket->write(data,size);
    m_socket->waitForBytesWritten();
}

void HandleMsg::addMsg(char* data, size_t size)
{
    qDebug()<<"发送数据1->"<<data;
    m_sendData.append(data,size);
}

void HandleMsg::addMsg(QString &strData)
{
    qDebug()<<"发送数据2->"<<strData;
    m_sendData.append(strData);
}

void HandleMsg::sendAllMsg()
{
    SlotSend();
}

bool HandleMsg::ReqLogin(const char *account, const char *password, const char *securitycode, const char *machinecode)
{
    //-------------------------- - 登录信息(发送)------------------------
    // 发送数据(登录消息)
    go::Login userLoginInfo;
    userLoginInfo.set_account(account);
    userLoginInfo.set_password(password);
    userLoginInfo.set_securitycode(securitycode);//token
    userLoginInfo.set_machinecode(machinecode);

    // len + id + data
    int len = userLoginInfo.ByteSize();
    int id = m_mapMsgID[typeid(go::Login).name()];
    char data[256];						//这里的大小可以适时调整
    userLoginInfo.SerializeToArray(data, len);

    // 数据序列化
    size_t size = 0;
    char* buffer = marshal(len, id, data, size);

    // 数据发送
    //sendMsg(buffer, size);
    sendIMMsg(buffer, size);
    qDebug()<<"登录信息成功发送\n";
    return true;
}

bool HandleMsg::ReqEnterRoom(int roomnumber, const char *key)
{
    // 发送数据(登录房间)
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    go::ReqEnterRoom enterRoom;
    enterRoom.set_roomnum(roomnumber);
    enterRoom.set_roomkey(key);


    // len + id + data
    int len = enterRoom.ByteSize();
    int id = m_mapMsgID[typeid(go::ReqEnterRoom).name()];
    char data[256];						//这里的大小可以适时调整
    enterRoom.SerializeToArray(data, len);

    // 数据序列化
    size_t size = 0;
    char* buffer = marshal(len, id, data, size);

    // 数据发送
    sendIMMsg(buffer, size);
    qDebug()<<"登录房间信息 成功发送\n";
    return true;
}

bool HandleMsg::ReqEnterGame(int gameID)
{
    // 填充数据
    go::ReqEnterGame enterGame;
    enterGame.set_gameid(gameID);

    // len + id + data
    int len = enterGame.ByteSize();
    int id = m_mapMsgID[typeid(go::ReqEnterGame).name()];
    char data[256];						//这里的大小可以适时调整
    enterGame.SerializeToArray(data, len);

    // 数据序列化
    size_t size = 0;
    char* buffer = marshal(len, id, data, size);

    // 数据发送
    //sendMsg(buffer, size);
    sendIMMsg(buffer, size);
    qDebug()<<"登录游戏 成功发送\n";
    return true;
}

bool HandleMsg::ReqPlayBet(int area, float money)
{
    go::GameBet bet;
    bet.set_betarea(area);
    bet.set_betscore(money);

    // len + id + data
    int len = bet.ByteSize();
    int id = m_mapMsgID[typeid(go::GameBet).name()];
    char data[256];						//这里的大小可以适时调整
    bet.SerializeToArray(data, len);

    // 数据序列化
    size_t size = 0;
    char* buffer = marshal(len, id, data, size);

    // 数据发送
    //sendMsg(buffer, size);
    sendIMMsg(buffer, size);
    qDebug()<<"下注区域:"<<area<<" 金币:"<<money<<" 成功发送\n";
    return true;
}

bool HandleMsg::ReqExitGame(int gameID)
{
    // 填充数据
    go::ReqExitGame exitGame;
    exitGame.set_gameid(gameID);

    // len + id + data
    int len = exitGame.ByteSize();
    int id = m_mapMsgID[typeid(go::ReqExitGame).name()];
    char data[256];						//这里的大小可以适时调整
    exitGame.SerializeToArray(data, len);

    // 数据序列化
    size_t size = 0;
    char* buffer = marshal(len, id, data, size);

    // 数据发送
    //sendMsg(buffer, size);
    sendIMMsg(buffer, size);
    qDebug()<<"退出游戏 成功发送\n";
    return true;
}




void HandleMsg::close()
{
    m_socket->close();
}

void HandleMsg::SlotSend()
{
    m_socket->write(m_sendData);
    m_socket->waitForBytesWritten();
    m_sendData.clear();
}

#include <QDataStream>
void HandleMsg::SlotRead()
{
    //parseData(m_socket->readAll().data(), m_socket->readAll().length());
    //m_socket->bytesAvailable()
    QByteArray allData = m_socket->readAll();
    qDebug()<<"接收长度:"<<allData.size() <<"  -> "<<m_socket->readBufferSize();

    //    QByteArray allData;
    //    while (!m_socket->atEnd()) {
    //            QByteArray data = m_socket->read(1024);
    //            allData.append(data);
    //        }

    emit recvSig( allData );
}

void HandleMsg::SlotError(QAbstractSocket::SocketError err)
{
    switch (err) {
    case QAbstractSocket::UnconnectedState:
        //QMessageBox::about(nullptr, tr("提示"), tr("无法连接服务端"));
        // emit m_socket->connected();
        break;
    case QAbstractSocket::HostLookupState:
        QMessageBox::about(nullptr, tr("提示"), tr("请检查是否与主机断开连接"));
        emit m_socket->connected();
        break;
    case QAbstractSocket::ConnectingState:
        QMessageBox::about(nullptr, tr("提示"), tr("正在连接主机"));
        break;
    case QAbstractSocket::ConnectedState:
        QMessageBox::about(nullptr, tr("提示"), tr("已经连接主机"));
        break;
    case QAbstractSocket::BoundState:
        // QMessageBox::about(nullptr, tr("提示"), tr("已经绑定"));
        break;
    case QAbstractSocket::ListeningState:
        //QMessageBox::about(nullptr, tr("提示"), tr("正在监听"));
        break;
    case QAbstractSocket::ClosingState:
        QMessageBox::about(nullptr, tr("提示"), tr("服务端已经关闭"));
        break;
    default:
        break;
    }
}

void HandleMsg::registerProtoMsg()
{
    //登录注册
    registerMsg(typeid(go::Login).name());
    registerMsg(typeid(go::ResResult).name());
    registerMsg(typeid(go::Register).name());
    registerMsg(typeid(go::ReqEnterRoom).name());
    registerMsg(typeid(go::ReqEnterGame).name());
    registerMsg(typeid(go::ReqExitGame).name());

    //游戏房间列表
    registerMsg(typeid(go::GameList).name());
    registerMsg(typeid(go::GameBet).name());
    registerMsg(typeid(go::GameHost).name());
    registerMsg(typeid(go::GameSuperHost).name());
    registerMsg(typeid(go::GameReady).name());

    // 子游戏
    baccaratRegister(); //百家乐

    //
}

void HandleMsg::registerMsg(const char * clsName)
{
    std::map<std::string, int>::iterator iter = m_mapMsgID.find(clsName);
    if (iter == m_mapMsgID.end())
    {//添加
        int value = m_mapMsgID.size() + 1;
        m_mapMsgID.insert(std::map<std::string,int>::value_type(clsName, value));
    }
}

void HandleMsg::baccaratRegister()
{
    registerMsg(typeid(go::GameBaccaratEnter).name());       //入场
    registerMsg(typeid(go::GameBaccaratHost).name());        //抢庄
    registerMsg(typeid(go::GameBaccaratSuperHost).name());   //超级抢庄
    registerMsg(typeid(go::GameBaccaratBet).name());         //下注
    registerMsg(typeid(go::GameBaccaratBetResult).name());   //下注结果
    registerMsg(typeid(go::GameBaccaratOver).name());        //开奖
    // registerMsg(typeid(go::GameBaccaratCheckout).name());    //结算
}

void HandleMsg::mahjongRegister()
{
    registerMsg(typeid(go::GameMahjongEnter).name());       //入场
    registerMsg(typeid(go::GameMahjongPlayer).name());      //玩家信息
    registerMsg(typeid(go::GameMahjongBegins).name());      //开始
    registerMsg(typeid(go::GameMahjongOutcard).name());     //出牌
    registerMsg(typeid(go::GameMahjongOperate).name());     //操作
    registerMsg(typeid(go::GameMahjongAward).name());       //个人得分
    registerMsg(typeid(go::GameMahjongCheckout).name());    //所有得分
}

std::string HandleMsg::parseData(const char *data, size_t size)
{
    // 数据反序列化
    m_theme = 0;
    m_state = 0;
    size_t nRealSize = 0;
    char* recvBuffer = unmarshal(data, size, nRealSize);
    if (NULL == recvBuffer)
        return  std::string("");

    // 分解数据
    go::PacketData packet;
    if (!packet.IsInitialized() || !packet.ParseFromArray(recvBuffer, nRealSize))
    {
        return  std::string("");
    }

    // 需要重复拆包


    // 主题码 和 状态码
    m_theme = packet.mainid();
    m_state = packet.subid();

    // 实际可用数据
    return packet.transdata();

}

