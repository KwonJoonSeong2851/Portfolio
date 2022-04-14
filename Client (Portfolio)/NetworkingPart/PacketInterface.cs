
using System;
using System.IO;

public interface PacketInterface 
{
    void Encode();
    void Decode(byte[] packet, ref int offset);


    Int64 GetType();
    MemoryStream GetStream();
}

public class PacketData
{
    protected MemoryStream m_packet = new MemoryStream();

    ~PacketData()
    {
        m_packet = null;
    }
}