#pragma once
#include "../stdafx.h"


//-------------------------------------------------//
/*

Singleton Å¬·¡½º

*/
//-------------------------------------------------//




template <class T>
class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}


public:
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);

	static T& GetInstance()
	{
		static T instance;

		return instance;
	}
};