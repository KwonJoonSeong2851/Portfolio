#include "../stdafx.h"
#include "TerminalSession.h"

bool TerminalSession::ConnectTo(char* ip, int port)
{
	m_socketData.socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketData.socket_ == INVALID_SOCKET)
	{
		SLOG(L"TerminalSession : terminal socket fail !!!");
		return false;
	}

	ZeroMemory(&m_socketData.addrInfo_, sizeof(m_socketData.addrInfo_));
	m_socketData.addrInfo_.sin_family = AF_INET;
	m_socketData.addrInfo_.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(m_socketData.addrInfo_.sin_addr));

	this->SetSocketOpt();

	int ret = ::connect(m_socketData.socket_, (sockaddr*)&m_socketData.addrInfo_, sizeof(m_socketData.addrInfo_));

	if (ret == SOCKET_ERROR)
	{
		SLOG(L"TerminalSession : terminal socket connect fail !!!");
		return false;
	}
	return true;
}

void TerminalSession::OnSend(size_t transferSize)
{

}

void TerminalSession::SendPacket(Packet* packet)
{
	Stream stream;
	packet->Serialize(stream);

	packet_size_t offset = 0;
	array<char, SOCKET_BUF_SIZE> buffer;
	packet_size_t packetLen[1] = { sizeof(packet_size_t) + (packet_size_t)stream.GetSize(), };

	memcpy_s(buffer.data() + offset, buffer.max_size(), (void*)packetLen, sizeof(packetLen));
	offset += sizeof(packetLen);

	memcpy_s(buffer.data() + offset, buffer.max_size(), stream.GetData(), packetLen[0]);
	offset += (packet_size_t)stream.GetSize();

	::send(m_socketData.socket_, buffer.data(), offset, 0);
}

Package* TerminalSession::OnRecv(size_t transferSize)
{
	array<char, SOCKET_BUF_SIZE> rowData;

	int ret = ::recv(m_socketData.socket_, rowData.data(), (int)rowData.size(), 0);
	if(ret <= 0)
	{
		return nullptr;
	}

	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };

	memcpy_s((void*)packetLen, sizeof(packetLen), (void*)rowData.data(), sizeof(packetLen));
	offset += sizeof(packetLen);

	//서버간 패킷 처리
	Packet* packet = PacketAnalyzer::GetInstance().Analyzer(rowData.data() + offset, packetLen[0]);

	if (packet == nullptr)
	{
		return nullptr;
	}

	Package* package = new Package(this, packet);
	return package;
}
