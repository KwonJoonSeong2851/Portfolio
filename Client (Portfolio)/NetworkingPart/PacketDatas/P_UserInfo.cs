using System;
using System.IO;

public class PK_C_REQ_USERINFO : PacketData,PacketInterface
{
    public UInt64 account;
    Int64 PacketInterface.GetType() { return (Int64)PacketType.E_C_REQ_USERINFO; }
    Int64 GetType() { return (Int64)PacketType.E_C_REQ_USERINFO; }

    void PacketInterface.Encode()
    {
        PacketUtil.EncodeHeader(m_packet, this.GetType());
        PacketUtil.Encode(m_packet, account);
    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {

    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }
}

public class PK_I_ANS_USERINFO : PacketData,PacketInterface
{
   public N_UserInfo userInfo;

    Int64 PacketInterface.GetType() { return (Int64)PacketType.E_I_ANS_USERINFO; }
    Int64 GetType() { return (Int64)PacketType.E_I_ANS_USERINFO; }
    void PacketInterface.Encode()
    {

    }

    void PacketInterface.Decode(byte[] packet, ref int offset)
    {
        userInfo.account = PacketUtil.DecodeUInt64(packet, ref offset);
        userInfo.id = PacketUtil.DecodeString(packet,ref offset);
        userInfo.nickName = PacketUtil.DecodeString(packet, ref offset);
        userInfo.level = PacketUtil.DecodeUInt32(packet, ref offset);
        userInfo.experience = PacketUtil.DecodeUInt64(packet, ref offset);
        userInfo.win = PacketUtil.DecodeUInt32(packet, ref offset);
        userInfo.lose = PacketUtil.DecodeUInt32(packet, ref offset);
    }

    MemoryStream PacketInterface.GetStream()
    {
        return m_packet;
    }

}
