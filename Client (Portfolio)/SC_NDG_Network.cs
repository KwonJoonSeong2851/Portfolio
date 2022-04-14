using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading.Tasks;

public class SC_NDG_Network : Singleton<SC_NDG_Network>
{
    public string SERVER_ADDRESS = "118.67.132.48";
    public string SUB_SERVER_ADDRESS = "wns7194.iptime.org";
    public int SERVER_PORT = 9000;

    static public Network network;
    static public UInt64 localPlayerAccount;
    static public N_UserInfo? localPlayerInfo;

    static private bool serverConnected = false;

    private void  Awake()
    {
        DontDestroyOnLoad(this.gameObject);
        Initialize();
    }


    public void Initialize()
    {
        network = Network.Instance;

        if (network == null)
        {
            Debug.Log("NDG Server 초기화 실패");
            return;
        }

        bool ret = network.Connect(SERVER_ADDRESS, SERVER_PORT, Network.ConnectionType.TCP);

        if (ret == false)
        {
            Debug.Log("NDG Main Server 초기화 실패, Sub Server 연결 시도");
            ret = network.Connect(SUB_SERVER_ADDRESS, SERVER_PORT, Network.ConnectionType.TCP,true);

            if (ret == false)
            {
                Debug.Log("Sub Server 연결 실패");
                IsServerConnected = false;
                return;
            }
        }

        IsServerConnected = true;

    }

    public Network GetNetwork()
    {
        return network;
    }

    static public bool IsServerConnected
    {
        get
        {
            return serverConnected;
        }
        set
        {
            serverConnected = value;
        }
    }


    public void TestGetUserInfo()
    {
        PK_C_REQ_USERINFO packet = new PK_C_REQ_USERINFO();
        packet.account = localPlayerAccount;
        network.SendReliable(packet);
        network.RegisterReceiveNotification(PacketType.E_I_ANS_USERINFO, OnReceiveUserInfo);
    }

    public void OnReceiveUserInfo(PacketType type,PacketInterface rowPacket)
    {
        network.UnregiseterReceiveNotification(PacketType.E_I_ANS_USERINFO);
        PK_I_ANS_USERINFO packet = (PK_I_ANS_USERINFO)rowPacket;
        packet.userInfo.level = SC_NDG_Network.Instance.GetLevel(packet.userInfo.experience);
        localPlayerInfo = packet.userInfo;
        Debug.Log("닉네임:"+ localPlayerInfo?.nickName+" 레벨:"+ localPlayerInfo?.level);
    }

 

}
