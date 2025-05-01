#include "StartScene.h"
#include "GameFrameWork/Application.h"
#include "GameScene.h"
#include <windows.h>
#include <cstdlib>   // 包含rand()函数
#include <ctime>     // 包含time()函数，用于初始化随机数种子
#include <graphics.h> // 确保包含EasyX的graphics.h
#include <math.h>     // 包含数学函数
#include <string>     // 添加字符串处理头文件
#include "TextUtils.h" // 添加自定义文本工具头文件

// Constructor
StartScene::StartScene(void)
    : m_bgImg(nullptr)
{
    // 初始化随机数种子
    srand((unsigned int)time(NULL));
}

// Draw function
void StartScene::draw()
{
    // Get screen dimensions
    int width = sApp->width();
    int height = sApp->height();
    
    // Draw gradient background
    for (int y = 0; y < height; y++)
    {
        float ratio = (float)y / height;
        int r = 10 + (int)(25 * ratio);
        int g = 5 + (int)(5 * ratio);
        int b = 40 + (int)(50 * ratio);
        
        setlinecolor(RGB(r, g, b));
        line(0, y, width, y);
    }
    
    // Draw stars
    for (int starIndex = 0; starIndex < 100; starIndex++)
    {
        int x = rand() % width;
        int y = rand() % height;
        int size = rand() % 3 + 1;
        int brightness = rand() % 155 + 100;
        
        setfillcolor(RGB(brightness, brightness, brightness));
        solidcircle(x, y, size);
    }
    
    // Draw nebula effects
    for (int cloudIndex = 0; cloudIndex < 5; cloudIndex++)
    {
        int x = rand() % width;
        int y = rand() % height;
        int radius = rand() % 80 + 20;
        
        for (int r = radius; r > 0; r -= 5)
        {
            int alpha = 255 - (radius - r) * 10;
            if (alpha < 30) 
            {
                alpha = 30;
            }
            
            int rr = 50 + rand() % 50;
            int rg = 30 + rand() % 40;
            int rb = 100 + rand() % 155;
            
            setfillcolor(RGB(rr, rg, rb));
            setlinestyle(PS_NULL);
            setfillstyle(BS_SOLID);
            fillcircle(x, y, r);
        }
    }
    
    // Draw UI elements
    drawTitle();
    drawPrompt();
    drawVersion();
}

// Helper functions to draw text
void StartScene::drawTitle()
{
    // 设置字体
    LOGFONT font;
    MyGetTextStyle(&font);               // 使用自定义函数获取当前字体
    font.lfHeight = 48;                // 设置字体高度
    
    // 使用memcpy而不是strncpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // 确保以空字符结尾
    
    MySetTextStyle(&font);               // 使用自定义函数设置字体样式
    setbkmode(TRANSPARENT);            // 设置背景透明
    
    const char* title = "Space Chess";
    std::string titleStr(title);
    
    // 手动计算文本宽度 (假设每个字符宽度为24像素)
    int titleWidth = static_cast<int>(titleStr.length()) * 24;
    int titleX = sApp->width() / 2 - titleWidth / 2;
    int titleY = sApp->height() / 2 - 50;
    
    // 转换为宽字符串
    std::wstring wideTitle = AnsiToWide(title);
    
    // 使用自定义函数绘制文本
    MyDrawText(
        titleX, 
        titleY, 
        wideTitle.c_str(), 
        RGB(255, 255, 255)
    );
}

void StartScene::drawPrompt()
{
    // 设置字体
    LOGFONT font;
    MyGetTextStyle(&font);               // 使用自定义函数获取当前字体
    font.lfHeight = 24;                // 设置字体高度
    
    // 使用memcpy而不是strncpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // 确保以空字符结尾
    
    MySetTextStyle(&font);               // 使用自定义函数设置字体样式
    
    const char* prompt = "Click to Start Game";
    
    // 手动计算文本宽度
    int promptWidth = static_cast<int>(strlen(prompt)) * 12; // 英文字符宽度约为12像素
    
    // 转换为宽字符串
    std::wstring widePrompt = AnsiToWide(prompt);
    
    // 使用自定义函数绘制文本
    MyDrawText(
        sApp->width() / 2 - promptWidth / 2, 
        sApp->height() / 2 + 20, 
        widePrompt.c_str(), 
        RGB(200, 200, 200)
    );
}

void StartScene::drawVersion()
{
    // 设置字体
    LOGFONT font;
    MyGetTextStyle(&font);               // 使用自定义函数获取当前字体
    font.lfHeight = 16;                // 设置字体高度
    
    // 使用memcpy而不是strncpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // 确保以空字符结尾
    
    MySetTextStyle(&font);               // 使用自定义函数设置字体样式
    
    const char* versionStr = "Version 1.0";
    
    // 手动计算文本宽度
    int versionWidth = static_cast<int>(strlen(versionStr)) * 8; // 英文小字符宽度约为8像素
    
    // 转换为宽字符串
    std::wstring wideVersion = AnsiToWide(versionStr);
    
    // 使用自定义函数绘制文本
    MyDrawText(
        sApp->width() - versionWidth - 20, 
        sApp->height() - 30, 
        wideVersion.c_str(), 
        RGB(150, 150, 150)
    );
}

// Update function
void StartScene::update()
{
    // 鼠标左键点击开始游戏
    if (sApp->msg()->message == WM_LBUTTONDOWN)
    {
        sApp->runScene(new GameScene());
    }
} 