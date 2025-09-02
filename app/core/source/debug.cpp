#include "pch.h"

#ifdef ON_DEBUG_STRINGS
void DbgMsg(const char* fmt, ...)
{

	va_list va;
	va_start(va, fmt);

	char buf[512] = { 0 };
	int len = vsprintf_s(buf, fmt, va); // print to buf fmt and args after fmt

	if (len <= sizeof(buf) - 2)
	{
		buf[len] = '\n';
		buf[len + 1] = 0;
	}
	else
		buf[sizeof(buf) - 1] = 0;

	OutputDebugStringA(buf);
	va_end(va);

}
#endif