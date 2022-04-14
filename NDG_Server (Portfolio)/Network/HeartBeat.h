#pragma once
#include "../stdafx.h"







class SessionMonitor : public Work
{
public :
	SessionMonitor();
	void Tick();
};
 
static SessionMonitor sessionMonitor;