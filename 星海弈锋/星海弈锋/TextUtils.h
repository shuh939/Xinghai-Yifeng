#pragma once
#include <Windows.h>
#include <graphics.h>
#include <string>

// 设置字体样式 - 替代settextstyle函数
inline void MySetTextStyle(LOGFONT* font) {
    // 使用Windows GDI的CreateFontIndirect创建字体
    HFONT hFont = CreateFontIndirect(font);
    if (hFont) {
        HDC hdc = GetImageHDC(NULL);
        SelectObject(hdc, hFont);
        DeleteObject(hFont); // 字体被选入DC后可以删除
    }
}

// 获取当前字体 - 替代gettextstyle函数
inline void MyGetTextStyle(LOGFONT* font) {
    // 简化版本 - 使用默认设置
    if (font) {
        // 基本默认属性
        font->lfHeight = 16;
        font->lfWidth = 0;
        font->lfEscapement = 0;
        font->lfOrientation = 0;
        font->lfWeight = FW_NORMAL;
        font->lfItalic = 0;
        font->lfUnderline = 0;
        font->lfStrikeOut = 0;
        font->lfCharSet = DEFAULT_CHARSET;
        font->lfOutPrecision = OUT_DEFAULT_PRECIS;
        font->lfClipPrecision = CLIP_DEFAULT_PRECIS;
        font->lfQuality = DEFAULT_QUALITY;
        font->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
        memcpy(font->lfFaceName, "黑体", sizeof(TCHAR) * 2);
        font->lfFaceName[2] = '\0';
    }
}

// 绘制文本 - 替代outtextxy函数
inline void MyDrawText(int x, int y, const wchar_t* text, COLORREF color) {
    HDC hdc = GetImageHDC(NULL);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    TextOutW(hdc, x, y, text, lstrlenW(text));
}

// 绘制单个字符
inline void MyDrawChar(int x, int y, wchar_t ch, COLORREF color) {
    wchar_t text[2] = { ch, L'\0' };
    MyDrawText(x, y, text, color);
}

// ANSI字符串转宽字符串(Unicode)
inline std::wstring AnsiToWide(const char* ansiStr) {
    if (!ansiStr || *ansiStr == 0) return L"";
    
    // 计算缓冲区大小
    int bufSize = MultiByteToWideChar(CP_ACP, 0, ansiStr, -1, NULL, 0);
    if (bufSize <= 0) return L"";
    
    // 分配宽字符缓冲区
    std::wstring wideStr(bufSize, L'\0');
    
    // 转换
    MultiByteToWideChar(CP_ACP, 0, ansiStr, -1, &wideStr[0], bufSize);
    
    // 移除结尾的空字符
    wideStr.resize(wcslen(wideStr.c_str()));
    
    return wideStr;
} 