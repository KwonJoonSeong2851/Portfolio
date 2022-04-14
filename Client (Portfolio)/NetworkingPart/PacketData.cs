using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class PK_C_REQ_CHATTING2 : PacketData,PacketInterface
{


    public Util_EnumState.CharacterType characterType;
    public Util_EnumState.Team team;
    public Util_EnumState.ChatSession session;

    public int roomNumber;

    public string m_id;
    public string m_text;

    Int64 PacketInterface.GetType()
    {
        return (Int64)PacketType.E_C_REQ_CHATTING;
    }

    Int64 GetType()
    {
        return (Int64)PacketType.E_C_REQ_CHATTING;
    }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet,(Int32)characterType);
        PacketUtil.Encode(m_packet, (Int32)team);
        PacketUtil.Encode(m_packet, (Int32)session);
        PacketUtil.Encode(m_packet, roomNumber);
        PacketUtil.Encode(m_packet, m_id);
        PacketUtil.Encode(m_packet, m_text);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        characterType = (Util_EnumState.CharacterType)PacketUtil.DecodeInt32(packet, ref offset);
        team = (Util_EnumState.Team)PacketUtil.DecodeInt32(packet, ref offset);
        session = (Util_EnumState.ChatSession)PacketUtil.DecodeInt32(packet, ref offset);
        roomNumber = PacketUtil.DecodeInt32(packet, ref offset);


        m_id = PacketUtil.DecodeString(packet, ref offset);
        m_text = PacketUtil.DecodeString(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }


}

public class PK_S_ANS_CHATTING : PacketData,PacketInterface
{
    public Util_EnumState.CharacterType characterType;
    public Util_EnumState.Team team;
    public Util_EnumState.ChatSession session;

    public int roomNumber;

    public string m_id;
    public string m_text;

    Int64 PacketInterface.GetType() { return (Int64)PacketType.E_S_ANS_CHATTING; }
    Int64 GetType() { return (Int64)PacketType.E_S_ANS_CHATTING; }

    void PacketInterface.Encode()
    {
       
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, (Int32)characterType);
        PacketUtil.Encode(m_packet, (Int32)team);
        PacketUtil.Encode(m_packet, (Int32)session);
        PacketUtil.Encode(m_packet, roomNumber);
        PacketUtil.Encode(m_packet, m_id);
        PacketUtil.Encode(m_packet, m_text);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        characterType = (Util_EnumState.CharacterType)PacketUtil.DecodeInt32(packet, ref offset);
        team = (Util_EnumState.Team)PacketUtil.DecodeInt32(packet, ref offset);
        session = (Util_EnumState.ChatSession)PacketUtil.DecodeInt32(packet, ref offset);
        roomNumber = PacketUtil.DecodeInt32(packet, ref offset);


        m_id = PacketUtil.DecodeString(packet, ref offset);
        m_text = PacketUtil.DecodeString(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }


}

public class PK_S_ANS_TEST : PacketData,PacketInterface
{
    public string m_testText;

    Int64 PacketInterface.GetType() { return (Int64)PacketType.E_S_ANS_TEST; }
    Int64 GetType() { return (Int64)PacketType.E_S_ANS_TEST; }
    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, m_testText);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        m_testText = PacketUtil.DecodeString(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }
}