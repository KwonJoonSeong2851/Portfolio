using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;


public class NetConfig
{
    public int PACKET_SIZE = 1024;
    public static int SERVER_PORT = 9000;
}
public enum NetEventType
{
    Connect = 0,
    Disconnect,
    SendError,
    ReceiveError,
}

public enum NetEventResult
{
    Failure = -1,
    Success = 0,
}

public class NetEventState
{
    public NetEventType type;
    public NetEventResult result;
    public IPEndPoint endPoint;
}