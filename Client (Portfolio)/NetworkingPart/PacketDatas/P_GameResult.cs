using System;
using System.IO;

public class PK_C_REQ_GAMERESULT : PacketData,PacketInterface
{
    public UInt64 account;
    public UInt32 gameResult;
    public UInt64 experience;

    Int64 PacketInterface.GetType() { return (Int64)PacketType.E_C_REQ_GAMERESULT; }
    Int64 GetType() { return (Int64)PacketType.E_C_REQ_GAMERESULT; }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, account);
        PacketUtil.Encode(m_packet, gameResult);
        PacketUtil.Encode(m_packet, experience);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {

    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }
  
}
