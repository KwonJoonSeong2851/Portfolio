using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

/// <summary>
/// PK_C_REQ_ID_PW
/// PK_I_ANS_ID_PW
/// 
/// 
/// </summary>
public class PK_C_REQ_ID_PW : PacketData,PacketInterface
{
    public string userId;
    public string userPassword;

    Int64 PacketInterface.GetType()
    {
        return (Int64)PacketType.E_C_REQ_ID_PW;
    }

    Int64 GetType()
    {
        return (Int64)PacketType.E_C_REQ_ID_PW;

    }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, userId);
        PacketUtil.Encode(m_packet, userPassword);

    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        userId = PacketUtil.DecodeString(packet, ref offset);
        userPassword = PacketUtil.DecodeString(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }

}

public class PK_I_ANS_ID_PW : PacketData,PacketInterface
{
    public UInt64 accountId;
    public bool result;

    Int64 PacketInterface.GetType()
    {
        return (Int64)PacketType.E_I_ANS_ID_PW;
    }

    Int64 GetType()
    {
        return (Int64)PacketType.E_I_ANS_ID_PW;

    }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, accountId);
        PacketUtil.Encode(m_packet, result);

    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        accountId = PacketUtil.DecodeUInt64(packet, ref offset);
        result = PacketUtil.DecodeBoolean(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }

}

public class PK_C_REQ_SIGNIN : PacketData,PacketInterface
{
    public string userId;
    public string userPwd;
    public string userNickName;

    Int64 PacketInterface.GetType()
    {
        return (Int64)PacketType.E_C_REQ_SIGNIN;
    }

    Int64 GetType()
    {
        return (Int64)PacketType.E_C_REQ_SIGNIN;
    }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, userId);
        PacketUtil.Encode(m_packet, userPwd);
        PacketUtil.Encode(m_packet, userNickName);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        userId = PacketUtil.DecodeString(packet, ref offset);
        userPwd = PacketUtil.DecodeString(packet, ref offset);
        userNickName = PacketUtil.DecodeString(packet, ref offset);
    }
    
    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }
}

public class PK_I_ANS_SIGNIN : PacketData,PacketInterface
{
    public int result;

    Int64 PacketInterface.GetType()
    {
        return (Int64)PacketType.E_I_ANS_SIGNIN;
    }

    Int64 GetType()
    {
        return (Int64)PacketType.E_I_ANS_SIGNIN;
    }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, (Int64)this.GetType());
        PacketUtil.Encode(m_packet, result);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        result = PacketUtil.DecodeInt32(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }

}