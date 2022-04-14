#pragma once
#include "../stdafx.h"


//-------------------------------------------------//
/*

Singleton Ŭ����

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