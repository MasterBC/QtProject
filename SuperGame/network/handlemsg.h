﻿////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): handlemsg.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-04-26
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////

#ifndef HANDLEMSG_H
#define HANDLEMSG_H

#include <QObject>
#include <QAbstractSocket>
#include "base/Magic_Singleton.h"


class QTcpSocket;
class HandleMsg:public QObject
{
    Q_OBJECT
public:
    explicit HandleMsg(QObject *parent = Q_NULLPTR);
    virtual ~HandleMsg();

    static HandleMsg* GetInstance();                                // 提供单例

    inline int GetTheme()const{ return m_theme; }
    inline int GetCode()const { return m_state; }                   //  状态码
    inline std::string GetUnpack()const{ return m_unpack; }                //  是否需要拆包


    bool connectTo(QString& strIP, int nPort, int msecs = 30000);   // 连接外网
    bool localHost(int nPort = 8080);//连接本地
    void sendIMMsg(char *data, size_t size);                        // 即时发送

    void addMsg(char *data, size_t size);                           // 刚连接时发送
    void addMsg(QString& strData);
    void sendAllMsg();                                              // 统一发送数据(暂时未优化为分包处理)
    std::string parseData(const char* data, size_t size);			// 数据解析

    // 请求登录
    bool ReqLogin(const char* account, const char* password, const char* securitycode, const char* machinecode);

    // 请求进入房间
    bool ReqEnterRoom(int roomnumber,const char* key);

    // 请求进入游戏
    bool ReqEnterGame(int gameID);

    // 请求准备(对战类游戏)
    bool ReqReady(int userID, bool isReady);

    // 请求下注
    bool ReqPlayBet(int area,float money);


    // 请求 出牌
    bool ReqPlayCard(int site, const char* cards, size_t len, const char* hints, size_t hintLen);


    // 请求退出游戏
    bool ReqExitGame(int gameID);//不需要服务端返回


    void close();

signals:
    void recvSig(QByteArray data);
    void disconnectSig();


public slots:
    void onSend();



private slots:
    void onRead();
    void onError(QAbstractSocket::SocketError err);
private:
    void registerProtoMsg();									// ******务必与leaf服务器注册的一致
    void registerMsg(const char* clsName);

    void baccaratRegister();
    void mahjongRegister();
    void landLordsRegister();

    int m_theme;                                                // 主题码  == mainID
    int m_state;												// 状态码  == subID
    std::string  m_unpack;                                      // 是否需要重复拆包
    size_t m_headSize;
    QTcpSocket* m_socket;
    QByteArray  m_sendData;
    std::map<std::string, int> m_mapMsgID;

};

#endif // HANDLEMSG_H
