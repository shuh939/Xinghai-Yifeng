#pragma once
#include <Windows.h>

// �Զ���GetTickCount64���ݺ��������ڲ�֧�ִ˺�����Windows�汾
inline ULONGLONG MyGetTickCount64()
{
    static ULONGLONG lastTickCount = 0;
    static DWORD lastLowPart = 0;
    static ULONGLONG highPart = 0;

    DWORD newLowPart = GetTickCount();

    // ������
    if (newLowPart < lastLowPart)
        highPart += 0x100000000ULL; // 2^32

    lastLowPart = newLowPart;
    lastTickCount = highPart + newLowPart;

    return lastTickCount;
}
