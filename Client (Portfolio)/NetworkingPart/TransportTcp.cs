using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System;


public class TransportTcp 
{
    private const int m_packetSize = 1024;

    private Socket m_listener = null;

    private List<Socket> m_socket = null;
    

    private bool m_isServer = false;
    private bool m_isConnected = false;


    //송신 버퍼
    private PacketQueue m_sendQueue = new PacketQueue();

    //수신 버퍼
    private PacketQueue m_recvQueue = new PacketQueue();

    //이벤트 통지 델리게이트
    public delegate void EventHandler(NetEventState state);

    //이벤트 핸들러
    private EventHandler m_handler;
     
    

    private System.Object lockObj = new System.Object();






    public TransportTcp()
    {
        m_socket = new List<Socket>();
    }


    /// <summary>
    /// 이벤트 등록
    /// </summary>
    public void RegisterEventHandler(EventHandler handler)
    {
        m_handler += handler;
    }

    /// <summary>
    /// 이벤트 삭제
    /// </summary>
    public void UnregisterEventHandler(EventHandler handler)
    {
        m_handler -= handler;
    }


    /// <summary>
    /// 접속
    /// <returns></returns>
    public bool Connect(string address, int port,bool DNS)
    {
        Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        try
        {

            socket.NoDelay = true;

            if (DNS == false)
            {
                IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse(address), port);
                socket.Connect(endPoint);
            } else
            {
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPHostEntry hostInfo = Dns.GetHostEntry(address);
                IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse(hostInfo.AddressList[0].ToString()), 9000);
                socket.Connect(endPoint);
            }
        }
        catch (SocketException e)
        {
            Debug.Log("Tcp : Connect fail :"+e.Message);
            return false;
        }

        if(socket != null && m_socket != null)
        m_socket.Add(socket);
        m_isConnected = true;
        return true;
    }


    /// <summary>
    /// 접속 해제
    /// </summary>
    public bool Disconnect()
    {
        try
        {
            if (m_socket != null)
            {
                foreach (Socket socket in m_socket)
                {
                    socket.Shutdown(SocketShutdown.Both);
                    socket.Close();
                }
                m_socket.Clear();
                m_socket = null;
            }

            if (m_handler != null)
            {
                NetEventState state = new NetEventState();
                state.type = NetEventType.Disconnect;
                state.result = NetEventResult.Success;
                m_handler(state);
            }
        }
        catch
        {
            return false;
        }

        Debug.Log("TransportTcp:: Disconnect");
        m_isConnected = false;
        return true;
    }



    /// <summary>
    /// 송신 데이터 Enqueue
    /// </summary>
    public int Send(byte[] data, int size)
    {
        return m_sendQueue.Enqueue(data, size);
    }

    /// <summary>
    /// 수신 데이터 Dequeue
    /// </summary>
    public int Receive(ref byte[] buffer, int size)
    {

        //m_recvQueue.Dequeue(ref buffer, size);

        //Int32 offset = 0;
        //Int32 packetLen = PacketUtil.DecodePacketLen(buffer, ref offset);

        //while (buffer.Length > 0 && packetLen < buffer.Length)
        //{
        //    m_recvQueue.Dequeue(ref buffer, size);
        //}

        return m_recvQueue.Dequeue(ref buffer, size);

    }



    void AcceptClient()
    {
        Debug.Log("AcceptClient.");
        if (m_listener != null && m_listener.Poll(0,SelectMode.SelectRead))
        {
            Socket socket = m_listener.Accept();
            m_socket.Add(socket);
            m_isConnected = true;
        }
    }
    public void Dispatch()
    {

        if(m_isConnected == true && m_socket != null)
        {
            lock(lockObj)
            {
                DispatchSend();

                DispatchReceive();
            }
        }
    }

    void DispatchSend()
    {
        if (m_socket == null)
            return;


        try
        {
            byte[] buffer = new byte[m_packetSize];

            int sendSize = m_sendQueue.Dequeue(ref buffer, buffer.Length);

            while (sendSize > 0)
            {
                foreach (Socket socket in m_socket)
                {
                    try
                    {
                        socket.Send(buffer, sendSize, SocketFlags.None);
                    } catch(Exception e)
                    {
                        Debug.Log("Tcp send error: " + e);
                    }
                }
                Debug.Log("Tcp : SendSize: " + sendSize);
                sendSize = m_sendQueue.Dequeue(ref buffer, buffer.Length);

            }

        }
        catch
        {
            if(m_handler != null)
            {
                NetEventState state = new NetEventState();
                state.type = NetEventType.SendError;
                state.result = NetEventResult.Failure;
                m_handler(state);
            }
        }
    }


    void DispatchReceive()
    {
        if (m_socket == null)
            return;

        try
        {
            foreach (Socket socket in m_socket)
            {
                if (socket.Poll(0, SelectMode.SelectRead))
                {
                    byte[] buffer = new byte[m_packetSize];
                    int offset = 0;
                    int readLen = socket.Receive(buffer, buffer.Length, SocketFlags.None);

                    int packetLen = PacketUtil.DecodePacketLen(buffer, ref offset);

                    if (readLen == 0)
                    {
                        Disconnect();
                    }
                    else if (readLen > 0)
                    {
                        while (readLen < packetLen)
                        {
                            byte[] remainPacket = new Byte[NetConfig.SERVER_PORT];
                            Int32 remainLen = 0;
                            remainLen = socket.Receive(remainPacket, remainPacket.Length, SocketFlags.None);
                            Buffer.BlockCopy(remainPacket, 0, buffer, readLen, remainLen);
                            readLen += remainLen;
                        }

                        Debug.Log("TransportTcp Receive : size[" + readLen + "]");

                        m_recvQueue.Enqueue(buffer, readLen);
                    }
                }
            }
        }
        catch
        {
            if (m_handler != null)
            {
                NetEventState state = new NetEventState();
                state.type = NetEventType.ReceiveError;
                state.result = NetEventResult.Failure;
                m_handler(state);
            }
        }
    }
 

    public bool IsConnected()
    {
        return m_isConnected;
    }


    public void StopServer()
    {
        Debug.Log("Tcp Stop");

        Disconnect();

        if (m_listener != null)
        {
            m_listener.Close();
            m_listener = null;

        }
        m_isServer = false;
    }
}
