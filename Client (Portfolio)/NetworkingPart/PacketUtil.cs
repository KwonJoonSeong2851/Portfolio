

using System;
using System.IO;
using System.Text;


public static class PacketUtil 
{
    public static void EncodeHeader(MemoryStream packet, Int64 headerType)
    {
        PacketUtil.Encode(packet, headerType);
    }


    public static void Encode(MemoryStream packet,Byte value)
    {
        packet.Write(BitConverter.GetBytes(value), 0, sizeof(Byte));
    }

    public static void Encode(MemoryStream packet, Boolean value)
    {
        packet.Write(BitConverter.GetBytes(value), 0, sizeof(Boolean));
    }
    public static void Encode(MemoryStream packet,Int32 value)
    {
        packet.Write(BitConverter.GetBytes(value), 0, sizeof(Int32));
    }

    public static void Encode(MemoryStream packet, UInt32 value)
    {
        packet.Write(BitConverter.GetBytes(value), 0, sizeof(UInt32));
    }

    public static void Encode(MemoryStream packet, Int64 value)
    {
        packet.Write(BitConverter.GetBytes(value), 0, sizeof(Int64));
    }

    public static void Encode(MemoryStream packet, UInt64 value)
    {
        packet.Write(BitConverter.GetBytes(value), 0, sizeof(UInt64));
    }

    public static void Encode(MemoryStream packet, string str)
    {
        int length = Encoding.UTF8.GetBytes(str).Length;
        Encode(packet, (Int32)length);
        
        packet.Write(Encoding.UTF8.GetBytes(str), 0, length);
    }
 

    //----------------------------------------------------------------------------------------------//


    public static Int32 DecodePacketLen(Byte[] data, ref Int32 offset)
    {
        return PacketUtil.DecodeInt32(data, ref offset);
    }

    public static Int64 DecodePacketType(Byte[] data,ref Int32 offset)
    {
        return PacketUtil.DecodeInt64(data, ref offset);
    }

    public static Byte DecodeByte(Byte[] data, ref Int32 offset)
    {
        Byte val = data[offset];
        offset += sizeof(Byte);
        return val;
    }

    public static Boolean DecodeBoolean(Byte[] data, ref Int32 offset)
    {
        Boolean val = BitConverter.ToBoolean(data, offset);
        offset += sizeof(Int32);
        return val;
    }
    public static Int32 DecodeInt32(Byte[] data, ref Int32 offset)
    {
        Int32 val = BitConverter.ToInt32(data, offset);
        offset += sizeof(Int32);
        return val;
    }

    public static UInt32 DecodeUInt32(Byte[] data, ref Int32 offset)
    {
        UInt32 val = BitConverter.ToUInt32(data, offset);
        offset += sizeof(Int32);
        return val;
    }

    public static Int64 DecodeInt64(Byte[] data, ref Int32 offset)
    {
        Int64 val = BitConverter.ToInt64(data, offset);
        offset += sizeof(Int64);
        return val;
    }

    public static UInt64 DecodeUInt64(Byte[] data, ref Int32 offset)
    {
        UInt64 val = BitConverter.ToUInt64(data, offset);
        offset += sizeof(Int64);
        return val;
    }

    public static string DecodeString(Byte[] data, ref Int32 offset)
    {
        Int32 strLen = PacketUtil.DecodeInt32(data, ref offset);
          
        string str = System.Text.Encoding.Unicode.GetString(data, offset, strLen);
        offset += strLen;
        return str;
    }

    public static PacketInterface PacketAnalyzer(Byte[] packetByte,ref Int32 offset, int packetLen)
    {
        Int64 packetType = PacketUtil.DecodePacketType(packetByte, ref offset);
        PacketInterface packet = PacketFactory.GetPacket(packetType);
        if(packet == null)
        {
            return null;
        }

        if(offset < packetLen)
        {
            packet.Decode(packetByte, ref offset);
        }
        return packet;
    }
}
