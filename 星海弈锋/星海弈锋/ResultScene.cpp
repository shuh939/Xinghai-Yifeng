#include "ResultScene.h"
#include "GameFrameWork/Application.h"
#include "StartScene.h"
#include <Windows.h>
#include "GameFrameWork/gui/Gui.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "ResManager.h"
#include <graphics.h> // 添加图形库头文件
#include <string> // 添加字符串处理头文件
#include "TimingUtils.h" // 添加自定义时间工具头文件
#include "TextUtils.h" // 添加自定义文本工具头文件

// 星空背景数据 - 使用全局变量但添加前缀避免冲突
const int RESULT_STAR_COUNT = 350;  // 星星数量
ResultStar resultStars[RESULT_STAR_COUNT]; // 使用ResultStar类型
DWORD resultLastUpdateTime = 0;    // 上次更新时间
float resultAnimationTime = 0;     // 动画时间累计

// 使用自己的GetTickCount64兼容函数定义已移至TimingUtils.h

ResultScene::ResultScene(ResultType resultType)
    : m_resultType(resultType)
{
    // 记录页面创建时间，使用兼容函数替代GetTickCount64
    m_createTime = MyGetTickCount64();
    
    // 初始化随机数种子
    srand((unsigned int)time(NULL));

    // 初始化星星
    for (int i = 0; i < RESULT_STAR_COUNT; i++)
    {
        // 位置完全随机
        resultStars[i].x = rand() % sApp->width();
        resultStars[i].y = rand() % sApp->height();

        // 大小随机，使一部分星星更大
        resultStars[i].size = (rand() % 100 < 85) ? (rand() % 2) + 1 : (rand() % 2) + 2;

        // 亮度基础值 - 确保有足够的亮度变化空间
        resultStars[i].baseBrightness = 150 + (rand() % 70);

        // 闪烁速度随机（值越小闪烁越慢）
        resultStars[i].twinkleSpeed = 0.5f + (rand() % 100) / 30.0f;

        // 闪烁相位随机（让每颗星星闪烁周期不同）
        resultStars[i].twinklePhase = (float)(rand() % 628) / 100.0f; // 0到2π之间

        // 星星颜色随机 - 大多数是白色，但有一些是淡蓝色、淡红色或淡黄色
        int colorRand = rand() % 100;
        if (colorRand < 60)
        {
            // 白色星星
            resultStars[i].color = RGB(0, 47, 167);
        }
        else if (colorRand < 75)
        {
            // 淡蓝色星星
            resultStars[i].color = RGB(252, 272, 10);
        }
        else if (colorRand < 95)
        {
            // 淡红色星星
            resultStars[i].color = RGB(238, 130, 238);
        }
        else if (colorRand < 105)
        {
            // 淡黄色星星
            resultStars[i].color = RGB(105, 105, 105);
        }
        else
        {
            // 淡红色星星
            resultStars[i].color = RGB(235, 92, 92);
        }
    }

    // 初始化时间
    resultLastUpdateTime = GetTickCount();
}

void ResultScene::draw()
{
    // 获取屏幕尺寸
    int width = sApp->width();
    int height = sApp->height();

    // 绘制深色背景 - 更暗的深蓝色背景，带有轻微变化
    for (int y = 0; y < height; y += 2) // 每两行绘制一次，提高性能
    {
        float ratio = (float)y / height;
        // 基础颜色更暗，更接近黑色
        int r = 2 + (int)(5 * ratio);
        int g = 2 + (int)(5 * ratio);
        int b = 10 + (int)(20 * ratio);

        // 在基础颜色上添加随机噪点，模拟星际尘埃
        if (rand() % 100 < 5)
        {
            r += rand() % 3;
            g += rand() % 3;
            b += rand() % 5;
        }

        setlinecolor(RGB(r, g, b));
        line(0, y, width, y);
        if (y + 1 < height)
        {
            line(0, y + 1, width, y + 1); // 填充下一行，提高性能
        }
    }

    // 绘制远景星群 - 小而密集的星点，不闪烁
    for (int i = 0; i < 300; i++)
    {
        int x = (i * 29) % width;
        int y = (i * 37) % height;
        int brightness = 50 + (rand() % 50);

        putpixel(x, y, RGB(brightness, brightness, brightness));
    }

    // 绘制闪烁的星星
    for (int i = 0; i < RESULT_STAR_COUNT; i++)
    {
        // 计算闪烁因子：正弦波形式
        float twinkleFactor = sin(resultAnimationTime * resultStars[i].twinkleSpeed + resultStars[i].twinklePhase);

        // 将闪烁因子转换为亮度变化（值在-40到40之间）
        int brightnessChange = (int)(twinkleFactor * 40.0f);

        // 计算当前亮度
        int currentBrightness = resultStars[i].baseBrightness + brightnessChange;
        if (currentBrightness < 100) currentBrightness = 100;
        if (currentBrightness > 255) currentBrightness = 255;

        // 获取基础颜色的RGB值
        int r = GetRValue(resultStars[i].color);
        int g = GetGValue(resultStars[i].color);
        int b = GetBValue(resultStars[i].color);

        // 根据当前亮度调整颜色
        float brightnessFactor = (float)currentBrightness / 255.0f;
        r = (int)(r * brightnessFactor);
        g = (int)(g * brightnessFactor);
        b = (int)(b * brightnessFactor);

        // 绘制星星
        setfillcolor(RGB(r, g, b));

        // 大星星有光晕效果
        if (resultStars[i].size >= 2)
        {
            // 先画一个稍淡的大圆作为光晕
            setfillcolor(RGB(r / 3, g / 3, b / 3));
            solidcircle(resultStars[i].x, resultStars[i].y, resultStars[i].size + 1);

            // 再画实心圆作为星星主体
            setfillcolor(RGB(r, g, b));
        }

        solidcircle(resultStars[i].x, resultStars[i].y, resultStars[i].size);
    }

    // 绘制UI元素 - 直接绘制渐变文本而不使用GUI_Label
    
    // 显示"YOU WIN!"文本（带渐变效果）
    const char* winText = "YOU WIN!";
    LOGFONT font;
    MyGetTextStyle(&font);           // 使用自定义函数代替gettextstyle
    font.lfHeight = 64;              // 增大字体高度
    font.lfWeight = FW_EXTRABOLD;    // 设置为特粗体(Extra Bold)
    
    // 修复strncpy警告，使用memcpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';       // 确保以空字符结尾
    
    MySetTextStyle(&font);           // 使用自定义函数代替settextstyle
    setbkmode(TRANSPARENT);          // 设置背景透明
    
    // 使用标准字符串计算宽度
    std::string winTextStr(winText);
    int winTextLen = static_cast<int>(winTextStr.length());
    
    // 手动计算文本宽度而不使用textwidth - 考虑到字体加大和加粗
    int textWidth = 0;
    for (int i = 0; i < winTextLen; i++) {
        // 为简单起见，这里假设每个字符宽度为32像素
        // 实际应用中可能需要更精确的计算方法
        textWidth += 32;
    }
    
    int textX = sApp->width() / 2 - textWidth / 2;
    int textY = sApp->height() / 2 - 60; // 稍微上移，以适应更大的字体
    
    // 计算基于时间的渐变参数
    float time = resultAnimationTime * 1.0f; // 加快渐变速度
    
    // 两个颜色之间渐变 - 使颜色更饱和一些
    int r1 = 100, g1 = 255, b1 = 100; // 更饱和的浅绿色
    int r2 = 100, g2 = 200, b2 = 255; // 更饱和的浅蓝色
    
    // 实现描边效果，先绘制黑色底
    int currentX = textX;
    for (int i = 0; i < winTextLen; i++)
    {
        // 绘制黑色底
        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                if (offsetX != 0 || offsetY != 0) {
                    // 绘制黑色描边
                    MyDrawChar(currentX + offsetX, textY + offsetY, (wchar_t)winTextStr[i], RGB(0, 0, 0));
                }
            }
        }
        
        // 位置递增
        currentX += 32;
    }
    
    // 对每个字符分别计算颜色和位置
    currentX = textX;
    for (int i = 0; i < winTextLen; i++)
    {
        // 计算每个字符的相位偏移，创建波浪渐变效果
        float phase = time + i * 0.8f; // 增加相位差，使波动更明显
        
        // 使用正弦波在两个颜色之间平滑过渡
        float ratio = (sin(phase) + 1.0f) * 0.5f; // 将正弦值(-1到1)映射到0到1
        
        // 计算当前字符的颜色
        int r = (int)(r1 + (r2 - r1) * ratio);
        int g = (int)(g1 + (g2 - g1) * ratio);
        int b = (int)(b1 + (b2 - b1) * ratio);
        
        // 使用自定义函数绘制单个字符
        MyDrawChar(currentX, textY, (wchar_t)winTextStr[i], RGB(r, g, b));
        
        // 每个字符宽度约为32像素 (因为字体加大了)
        currentX += 32;
    }
    
    // 显示提示文本
    font.lfHeight = 24;
    MySetTextStyle(&font);
    
    const char* promptText = "点击鼠标左键返回开始页面";
    
    // 避免使用textwidth
    int promptWidth = 24 * strlen(promptText) / 2; // 中文字符占2字节，英文字符占1字节
    
    // 转换为宽字符串
    std::wstring widePrompt = AnsiToWide(promptText);
    
    // 使用自定义函数绘制文本
    MyDrawText(
        sApp->width() / 2 - promptWidth / 2, 
        sApp->height() / 2 + 20, 
        widePrompt.c_str(), 
        RGB(200, 200, 200)
    );
    
    // 显示版本号
    font.lfHeight = 16;
    MySetTextStyle(&font);
    
    const char* versionText = "版本：1.0";
    
    // 避免使用textwidth
    int versionWidth = 16 * strlen(versionText) / 2; // 中文字符占2字节
    
    // 转换为宽字符串
    std::wstring wideVersion = AnsiToWide(versionText);
    
    // 使用自定义函数绘制文本
    MyDrawText(
        sApp->width() - versionWidth - 20, 
        sApp->height() - 30, 
        wideVersion.c_str(), 
        RGB(150, 150, 150)
    );
}

void ResultScene::update()
{
    // 计算时间增量（秒）
    DWORD currentTime = GetTickCount();
    float deltaTime = (currentTime - resultLastUpdateTime) / 1000.0f;
    resultLastUpdateTime = currentTime;

    // 更新动画时间
    resultAnimationTime += deltaTime;
    
    // 不再需要单独更新文字渐变效果，因为现在使用动画时间
    
    // 避免点击太快意外跳过结算页面，使用兼容函数替代GetTickCount64
    if (MyGetTickCount64() - m_createTime < 500)
    {
        return;
    }

    // 检测鼠标点击，点击后返回开始页面
    if (sApp->msg()->message == WM_LBUTTONDOWN)
    {
        sApp->runScene(new StartScene());
    }
}
