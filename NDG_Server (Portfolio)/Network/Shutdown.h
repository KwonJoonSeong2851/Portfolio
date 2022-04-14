#pragma once


static bool _shutdown = false;

static void ShutdownServer()
{
	_shutdown = true;
}