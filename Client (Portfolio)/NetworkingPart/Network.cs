using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;
using System.Runtime.InteropServices;
using System;
using System.IO;

public class Network : Singleton<Network>
{
    private TransportTcp m_tcp = null;

    private Thread m_thread = null;

    private bool m_isStarted = false;

    private bool m_isServer = false;

    private const int m_packetSize = 1024;

    private const int packetMax = (int)PacketId.Max;

    private PacketProcess m_packetProcess ;

    public delegate void RecvNotifier(PacketType id, PacketInterface rowPacket);

    private Dictionary<Int64, RecvNotifier> m_notifier = new Dictionary<Int64, RecvNotifier>();

    public delegate void EventHandler(NetEventState state);

    private EventHandler m_handler;

    private bool m_eventOccured = false;

    public enum ConnectionType
    {
        TCP = 0,
        UDP,
        Both,
    }

    private void Start()
    {
        DontDestroyOnLoad(gameObject);
        StartCoroutine(RecvCor());
    }

    private void Update()
    {
        
    }

    IEnumerator RecvCor()
    {
        while (true)
        {
            if (IsConnected() == true)
            {
                byte[] packet = new byte[m_packetSize];

                //TCP 수신 처리

                if (m_tcp != null)
                {
                    int recvSize = m_tcp.Receive(ref packet, packet.Length);

                    if (recvSize > 0)
                    {
                        ReceivePacket(packet, recvSize);
                    }
                }

            }
            yield return new WaitForSeconds(0.01f);
        }
    }

    public bool Connect(string address, int port, ConnectionType type, bool DNS = false)
    {
        try
        {
            Debug.Log("Network:: Connect Server  Address:" + address + " Port:" + port + " type:" + type.ToString());

            bool ret = true;
            if(type == ConnectionType.TCP || type == ConnectionType.Both)
            {
                if (m_tcp == null)
                {
                    m_tcp = new TransportTcp();
                    m_tcp.RegisterEventHandler(OnEventHandling);
                }
                ret &= m_tcp.Connect(address, port,DNS);
            }


            if (ret == false)
            {

                return false;
            }
        }
        catch (IndexOutOfRangeException e)
        {
            Debug.Log(e.Message);
            return false;
        }

        Debug.Log("Success connection to server.");
        return LaunchThread();
    }

    public bool Disconnect()
    {
        if (m_tcp != null)
        {
            m_tcp.Disconnect();
        }

        m_isStarted = false;
        m_eventOccured = false;

        return true;
    }

    public void RegisterReceiveNotification(PacketType id, RecvNotifier notifier)
    {
        int index = (int)id;
        if(m_notifier.ContainsKey(index))
        {
            m_notifier.Remove(index);
        }

        m_notifier.Add(index, notifier);
    }

    public void UnregiseterReceiveNotification(PacketType id)
    {
        int index = (int)id;

        if(m_notifier.ContainsKey(index))
        {
            m_notifier.Remove(index);
        }
    }


    /// <summary>
    /// TCP 송신
    /// </summary>
    public int SendReliable<T>(IPacket<T> packet)
    {
        int sendSize = 0;

        if(m_tcp != null)
        {
            Debug.Log("SendReliable");
            PacketHeader header = new PacketHeader();
            HeaderSerialize serializer = new HeaderSerialize();

            header.packetId = packet.GetPacketId();

            byte[] headerData = null;
            if(serializer.Serialize(header) == true)
            {
                headerData = serializer.GetSerializedData();
            } else
            {
                Debug.Log("serialize header error");
            }

            byte[] packetData = packet.GetData();
            byte[] data = new byte[headerData.Length + packetData.Length];

            int headerSize = Marshal.SizeOf(typeof(PacketHeader));
            Buffer.BlockCopy(headerData, 0, data, 0, headerSize);
            Buffer.BlockCopy(packetData, 0, data, headerSize, packetData.Length);

            sendSize = m_tcp.Send(data, data.Length);
        }
        return sendSize;
    }

    public void SendReliable(PacketInterface packet)
    {
        try
        {
            packet.Encode();
            MemoryStream packetData = new MemoryStream();

            Int32 packetLen = sizeof(Int32) + (Int32)packet.GetStream().Length; 

            packetData.Write(BitConverter.GetBytes(packetLen), 0, sizeof(Int32));
            packetData.Write(packet.GetStream().ToArray(), 0, (Int32)packet.GetStream().Length);

            Byte[] packetBytes = packetData.ToArray();
            m_tcp.Send(packetBytes, (int)packetData.Length);
        } catch(Exception e)
        {
            Debug.Log("Network : send error :" + e);
        }
    }


    /// <summary>
    /// 수신
    /// </summary>
    public void ReceivePacket(byte[] data,int recvSize)
    {
        try
        {
            Int32 offset = 0;
            Int32 packetLen = PacketUtil.DecodePacketLen(data,ref offset);

            //Byte[] packetData = new Byte[m_packetSize];

            //Buffer.BlockCopy(data, offset, packetData, 0, recvSize - offset);
            Debug.Log("recvSize: " + recvSize + " , packetLen: " + packetLen); 
            Debug.Log(offset);
          
                PacketInterface rowPacket = PacketUtil.PacketAnalyzer(data, ref offset, recvSize);

                if (rowPacket == null && this.IsConnected())
                {
                    Debug.LogError("Network : Recv Error");
                    return;
                }

                Int64 type = rowPacket.GetType();
                Debug.Log((PacketType)type);
                m_notifier[type]((PacketType)type, rowPacket);
            

        } catch (Exception e)
        {
            Debug.LogError("ReceivePacket Error :" + e);
            return;
        }
    }


    public bool IsConnected()
    {
        bool isTcpConnected = false;
        
        if(m_tcp != null && m_tcp.IsConnected())
        {
            isTcpConnected = true;
        }

        if(m_tcp != null)
        {
            return isTcpConnected;
        }

        return (isTcpConnected);
    }




    
    private bool LaunchThread()
    {
        try
        {
            m_thread = new Thread(new ThreadStart(Dispatch));
            m_thread.Start();
        }
        catch
        {
            Debug.Log("Cannot launch Thread");
            return false;
        }

        m_isStarted = true;
        Debug.Log("Thread launched");
        return true;
    }
    

    private void Dispatch()
    {
        while(m_isStarted == true)
        {
            if (m_tcp != null)
            {
                m_tcp.Dispatch();
            }

            Thread.Sleep(5);
        }
    }


    private void OnApplicationQuit()
    {
        if(m_isStarted == true)
        {
            StopServer();
        }
    }
    

    public void OnEventHandling(NetEventState state)
    {
        if (m_handler != null)
        {
            m_handler(state);
        }
    }

    public void StopServer()
    {
        m_isStarted = false;
        if(m_thread != null)
        {
            m_thread.Join();
            m_thread = null;
        }

        if(m_tcp != null)
        {
            m_tcp.StopServer();
        }

        m_notifier.Clear();
        m_isServer = false;
        m_eventOccured = false;

        Debug.Log("Server stopped.");
    }
}
