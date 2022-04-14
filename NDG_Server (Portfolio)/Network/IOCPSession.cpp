#include "../stdafx.h"
#include "Session.h"
#include "IOCPSession.h"
#include "SessionManager.h"
#include "Packet/PacketAnalyer.h"

IoData::IoData()
{
	ZeroMemory(&m_overlapped, sizeof(m_overlapped));
	m_ioType = IO_ERROR;

	this->Clear();
}


void IoData::Clear()
{
	m_buffer.fill(0);
	m_totalBytes = 0;
	m_currentBytes = 0;
}

bool IoData::NeedMoreIo(size_t transferSize)
{
	m_currentBytes += transferSize;
	if (m_currentBytes < m_totalBytes)
	{
		return true;
	}
	return false;
}

int32_t IoData::SetupTotalBytes()
{
	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };

	if (m_totalBytes == 0)
	{
		memcpy_s((void*)packetLen, sizeof(packetLen), (void*)m_buffer.data(), sizeof(packetLen));
		m_totalBytes = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}

size_t IoData::GetTotalBytes()
{
	return m_totalBytes;
}

IO_OPERATION& IoData::GetType()
{
	return m_ioType;
}

void IoData::SetType(IO_OPERATION type)
{
	m_ioType = type;
}

char* IoData::GetData()
{
	return m_buffer.data();
}

bool IoData::SetData(Stream& stream)
{
	this->Clear();

	if (m_buffer.max_size() <= stream.GetSize())
	{
		SLOG(L"IoData : packet size too big [%d]byte", stream.GetSize());
		return false;
	}

	packet_size_t offset = 0;
	char* buf = m_buffer.data();


	packet_size_t packetLen[1] = { sizeof(packet_size_t) + (packet_size_t)stream.GetSize(), };

	memcpy_s(buf + offset, m_buffer.max_size(), (void*)packetLen, sizeof(packetLen));
	offset += sizeof(packetLen);

	memcpy_s(buf + offset, m_buffer.max_size(), stream.GetData(), (int32_t)stream.GetSize());
	offset += (packet_size_t)stream.GetSize();

	m_totalBytes = offset;
	return true;
}

LPWSAOVERLAPPED IoData::GetOverlapped()
{
	return &m_overlapped;
}

WSABUF IoData::GetWSABuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = m_buffer.data() + m_currentBytes;
	wsaBuf.len = (ULONG)(m_totalBytes - m_currentBytes);
	return wsaBuf;
}






//-------------------------------------------------//



 


IOCPSession::IOCPSession() : Session()
{
	this->Initialize();
}

void IOCPSession::Initialize()
{
	ZeroMemory(&m_socketData, sizeof(SOCKET_DATA));
	m_ioData[IO_READ].SetType(IO_READ);
	m_ioData[IO_WRITE].SetType(IO_WRITE);
}

void IOCPSession::CheckErrorIO(DWORD ret)
{
	if (ret == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		SLOG(L"IOCPSession : socket error: %d", WSAGetLastError());
	}
}

void IOCPSession::Recv(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes;
	DWORD errorCode = WSARecv(m_socketData.socket_, &wsaBuf, 1, &recvBytes, &flags, m_ioData[IO_READ].GetOverlapped(), NULL);

	this->CheckErrorIO(errorCode);
}

bool IOCPSession::IsRecving(size_t transferSize)
{
	if (m_ioData[IO_READ].NeedMoreIo(transferSize))
	{
		this->Recv(m_ioData[IO_READ].GetWSABuf());
		return true;
	}
	return false;
}

void IOCPSession::RecvStandBy()
{
	m_ioData[IO_READ].Clear();

	WSABUF wsaBuf;
	wsaBuf.buf = m_ioData[IO_READ].GetData();
	wsaBuf.len = SOCKET_BUF_SIZE;

	this->Recv(wsaBuf);
}

void IOCPSession::Send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(m_socketData.socket_, &wsaBuf, 1, &sendBytes, flags, m_ioData[IO_WRITE].GetOverlapped(), NULL);
	this->CheckErrorIO(errorCode);

}

void IOCPSession::OnSend(size_t transferSize)
{
	if (m_ioData[IO_WRITE].NeedMoreIo(transferSize))
	{
		this->Send(m_ioData[IO_WRITE].GetWSABuf());
	}
}

void IOCPSession::SendPacket(Packet* packet)
{
	Stream stream;
	packet->Serialize(stream);
	
	if (!m_ioData[IO_WRITE].SetData(stream))
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = m_ioData[IO_WRITE].GetData();
	wsaBuf.len = (ULONG)stream.GetSize();

	this->Send(wsaBuf);
	this->RecvStandBy();
}

Package* IOCPSession::OnRecv(size_t transferSize)
{
	packet_size_t offset = 0;
	offset += m_ioData[IO_READ].SetupTotalBytes();

	if (this->IsRecving(transferSize))
	{
		return nullptr;
	}

	packet_size_t packetDataSize = m_ioData[IO_READ].GetTotalBytes() - sizeof(packet_size_t);
	Byte* packetData = (Byte*)m_ioData[IO_READ].GetData() + offset;

	//PacketObfuscation::GetInstance().decoding(packetData, packetDataSize);
	Packet* packet = PacketAnalyzer::GetInstance().Analyzer((const char*)packetData, packetDataSize);

	if (packet == nullptr)
	{
		SLOG(L"IOCPSession : Invalid Packet !!!");
		this->OnClose();
		return nullptr;
	}

	this->RecvStandBy();
	Package* package = new Package(this,packet);
	return package;
}