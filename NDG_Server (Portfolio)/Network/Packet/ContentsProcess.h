#pragma once
#include "../../stdafx.h"

#define MAX_PACKET_THREAD_ SIZE_64

class Thread;
class ContentsProcess
{
private:
	std::array<Thread*, MAX_PACKET_THREAD_> m_threadPool;
	ThreadJobQueue<Package*>* m_packageQueue;

protected:
	typedef void(*RunFunc)(Session* session, Packet* rowPacket);
	std::hash_map<PacketType, RunFunc> m_runFuncTable;

private:
	void Initialize(xml_t* config);
	void RegistDefaultPacketFunc();
	void Run(Package* package);
	void Execute();

	void Process();
public:
	ContentsProcess();
	~ContentsProcess();

	void PutPackage(Package* package);
	virtual void RegistSubPacketFunc() = 0;

	static void Packet_HeartBeat(Session* session, Packet* rowPacket) {};
	static void Packet_Notify_Terminal(Session* session, Packet* rowPacket) {};
	static void Packet_Exit(Session* session, Packet* rowPacket) {};
};