#pragma once
#include "stdafx.h"

class DBProcess : public ContentsProcess
{
public :
	DBProcess();

private:
	void RegistSubPacketFunc();
	static void IPacket_ID_PW(Session* session, Packet* rowPacket);


};