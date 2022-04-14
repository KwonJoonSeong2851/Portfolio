#pragma once
#include "../stdafx.h"


//-------------------------------------------------//
/*
     Accept된 Client 정보 관리
  
*/
//-------------------------------------------------//

struct SOCKET_DATA
{
    SOCKET        socket_;
    SOCKADDR_IN   addrInfo_;
};

enum
{
    SESSION_TYPE_TERMINAL,
    SESSION_TYPE_CLIENT,
};

class Package;
class Packet;
class User;

class Session
{
protected:
    SOCKET_DATA      m_socketData;
    oid_t            m_id;
    int8_t           m_type;
    tick_t           m_lastHeartBeat;

    bool             SetSocketOpt();
    User* m_user;

public:
    Session();
    virtual ~Session();
    virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);

    virtual void OnSend(size_t transferSize) = 0;
    virtual void SendPacket(Packet* packet)  = 0;

    virtual Package* OnRecv(size_t transferSize) = 0;
    virtual void RecvStandBy() {};
    virtual void OnClose();

    SOCKET& GetSocket();
    str_t GetClientAddress();

    oid_t GetId();
    void SetId(oid_t id); 

    int8_t GetType();
    void SetType(int8_t type);

    tick_t HeartBeat();
    void UpdateHeartBeat();

    void SetUser(User* user);
    User* GetUser();

};