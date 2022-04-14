#pragma once
#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"Winmm.lib")

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <mmsystem.h>
#include <WS2spi.h>
#include <mstcpip.h>


#include <Windows.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
//#include <stdio.h>
#include <stdint.h>
#include <ctime>

#include <assert.h>
#include <fcntl.h>
#include <algorithm>
#include <functional>
#include <array>
#include <map>

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
//#include <hash_set>

#include<thread>
#include<mutex>
#include<memory>


#include <string>
//#include <tchar.h>

#include <ctime>
#include <random>
#include <typeinfo>



#if _DEBUG
#define CONTEXT_SWITCH Sleep(1)
#else
#define CONTEXT_SWITCH ::  SwitchToThread()
#endif

typedef void(*Funcation)(void*);

//외부 유틸
#include "./Util/tinyxml/tinyxml.h"




//유틸
#include "./Util/Util.h"
#include "./Util/Type.h"
#include "./Util/Singleton.h"
#include "./Util/Config.h"
#include "./Util/Lock.h"
#include "./Util/Thread.h"
#include "./Util/Clock.h"
#include "./Util/Logger.h"
#include "./Util/Task.h"
#include "./Util/ThreadJobQueue.h"



//네트워크
#include "./Network/Server.h"
#include "./Network/ServerIOCP.h"
#include "./Network/Session.h"

#include "./Network/IOCPSession.h"
#include "./Network/SessionManager.h"
#include "./Network/HeartBeat.h"

#include "./Network/TerminalSession.h"
#include "./Network/Terminal.h"
#include "./Network/TerminalManager.h"


//패킷 처리
#include "./Network/Packet/Stream.h"
#include "./Network/Packet/PacketHeader.h"
#include "./Network/Packet/PacketClass.h"
#include "./Network/Packet/PacketAnalyer.h"
#include "./Network/Packet/PacketFactory.h"
#include "./Network/Packet/Package.h"

#include "./Network/Packet/Protocol.h"



//데이터 베이스

#include "Database/ADODatabase.h"
#include "Database/Query.h"
#include "Database/DBManager.h"




#include "Network/Shutdown.h"

