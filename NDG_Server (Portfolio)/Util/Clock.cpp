#include "../stdafx.h"
#include "Clock.h"



Clock::Clock()
{
	m_serverStartTick = this->SystemTick();
}

Clock::~Clock()
{
}

wstr_t Clock::TickToStr(tick_t tick,const WCHAR* fmt)
{
	
	array<WCHAR, SIZE_128> timeStr;


	tm time;
	localtime_s(&time, &tick);
	wcsftime(timeStr.data(), timeStr.size(), fmt, &time);

	return timeStr.data();
}

tick_t Clock::ServerStartTick()
{
	return m_serverStartTick;
}

tick_t Clock::SystemTick()
{
	return system_clock::to_time_t(system_clock::now());
}

tick_t Clock::StrToTick(wstr_t str, const WCHAR* fmt)
{
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	swscanf_s(str.c_str(), fmt, &year, &month, &day, &hour, &minute, &second);

	//          초       분    시    일 월(0부터 시작) , 년 (1900년부터 시작)
	tm time = { second,minute,hour,day,month - 1, year - 1900 };

	return mktime(&time);
}

wstr_t Clock::NowTime(const WCHAR* fmt)
{
	return this->TickToStr(this->SystemTick(), fmt);
}

wstr_t Clock::NowTimeWithMilliSec(const WCHAR* fmt)
{
#if 1
	timePoint now = system_clock::now();
	timePoint oldSecond = system_clock::from_time_t(this->SystemTick());

	duration<double> milliSecond = now - oldSecond;
	array<WCHAR, SIZE_8> milliStr;
	snwprintf(milliStr, L"%03d", (int)(milliSecond.count() * 1000));
#else
	high_resolution_clock::time_point point = high_resolution_clock::now();
	milliseconds ms = duration_cast<milliseconds>(point.time_since_epoch());

	seconds s = duration_cast<seconds>(ms);
	tick_t t = s.count();
	std::size_t fractionalSeconds = ms.count() % 1000;
	array<WCHAR, SIZE_8> milliStr;
	_snwprintf(milliStr, L"%03d", (int)(fractionalSeconds));
#endif
	wstr_t timeString = this->TickToStr(this->SystemTick(), fmt);
	timeString += L".";
	timeString += milliStr.data();
	return timeString;
}

wstr_t Clock::Today()
{
	return this->TickToStr(this->SystemTick(), DATE_FORMAT);
}

wstr_t Clock::tomorrow()
{
	return this->TickToStr(this->SystemTick() + DAY_TO_TICK(1), DATE_FORMAT);
}

wstr_t Clock::Yesterday()
{
	return this->TickToStr(this->SystemTick() - DAY_TO_TICK(1), DATE_FORMAT);
}

DayOfTheWeek Clock::TodayOfTheWeek()
{
	tm time;
	tick_t tick = this->SystemTick();
	localtime_s(&time, &tick);
	return ((DayOfTheWeek)time.tm_wday);
}
