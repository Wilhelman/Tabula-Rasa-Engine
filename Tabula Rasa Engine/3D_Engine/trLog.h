#ifndef __trLog_H__
#define __trLog_H__

#include <windows.h>
#include <stdio.h>

#define TR_LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__)

void log(const char file[], int line, const char* format, ...);

#endif