#include "HeartBeat.h"

SessionMonitor::SessionMonitor()
{
	static bool init = false;
	if (init)
	{
		return;
	}

	init = true;
	const int MONITOR_REPORTING_SEC = 1;
	//TaskManager::GetInstance().Add(this, MONITOR_REPORTING_SEC, TICK_INFINITY);
}

void SessionMonitor::Tick()
{


#ifdef _DEBUG
	const int MAX_HEART_BEAT = 10000;
#else
	const int MAX_HEART_BEAT = 100000;
#endif
	SAFE_LOCK(SessionManager::GetInstance().GetLock());
	auto list = SessionManager::GetInstance().GetSessionList();
	tick_t now = CLOCK.SystemTick();

	for (auto session : list)
	{
		if (session->GetType() != SESSION_TYPE_CLIENT)
			continue;

		tick_t lastTick = session->HeartBeat();
		if (now - lastTick > MAX_HEART_BEAT)
		{
			SLOG(L"SessionMonitor : Closing by heartBeat", session->GetClientAddress().c_str());
			session->OnClose();
		}
	}

	return;
}
