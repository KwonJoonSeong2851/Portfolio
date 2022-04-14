#pragma once
#include "../stdafx.h"

class Session;
class SessionManager;
class Package;
class Packet;
class Stream;

typedef enum
{
	IO_READ,
	IO_WRITE,
	IO_ERROR,
} IO_OPERATION;

#define IO_DATA_MAX (2)

class IoData
{
	OVERLAPPED m_overlapped;
	IO_OPERATION m_ioType;
	size_t m_totalBytes;
	size_t m_currentBytes;

	std::array<char, SOCKET_BUF_SIZE> m_buffer;

public:
	IoData();
	~IoData() {};
	void Clear();

	bool NeedMoreIo(size_t transferSize);
	int32_t SetupTotalBytes();
	size_t GetTotalBytes();

	IO_OPERATION& GetType();
	void SetType(IO_OPERATION type);

	WSABUF GetWSABuf();
	char* GetData();
	bool SetData(Stream& stream);
	LPWSAOVERLAPPED GetOverlapped();

};


//-------------------------------------------------//


class Session;
class IOCPSession : public Session
{
public:
	std::array<IoData, IO_DATA_MAX> m_ioData;

private:
	void Initialize();

	void CheckErrorIO(DWORD ret);

	void Recv(WSABUF wsaBuf);
	bool IsRecving(size_t transferSize);

	void Send(WSABUF wsaBuf);

public:
	IOCPSession();

	void OnSend(size_t transferSize);
	void SendPacket(Packet* packet);

	Package* OnRecv(size_t transferSize);
	void RecvStandBy();
};