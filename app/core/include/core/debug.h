#pragma once

#ifdef ON_DEBUG_STRINGS
void DbgMsg(const char* format, ...);

#else

#define DbgMsg(...) __noop

#endif