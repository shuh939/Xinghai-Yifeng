#pragma once
#include <Windows.h>

// 自定义GetTickCount64兼容函数，用于不支持此函数的Windows版本
inline ULONGLONG MyGetTickCount64()
{
    static ULONGLONG lastTickCount = 0;
    static DWORD lastLowPart = 0;
    static ULONGLONG highPart = 0;

    DWORD newLowPart = GetTickCount();

    // 检测溢出
    if (newLowPart < lastLowPart)
        highPart += 0x100000000ULL; // 2^32

    lastLowPart = newLowPart;
    lastTickCount = highPart + newLowPart;

    return lastTickCount;
}
