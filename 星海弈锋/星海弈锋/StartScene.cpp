#include "StartScene.h"
#include "GameFrameWork/Application.h"
#include "GameScene.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <graphics.h>
#include <math.h>
#include "ResManager.h"
#include <string> // 添加字符串处理头文件
#include "TimingUtils.h" // 添加通用时间工具头文件
#include "TextUtils.h" // 添加自定义文本工具头文件

// 星星结构体
struct Star
{
    int x, y;              // 位置
    int size;              // 大小
    int baseBrightness;    // 基础亮度
    float twinkleSpeed;    // 闪烁速度
    float twinklePhase;    // 闪烁相位
    COLORREF color;        // 星星颜色
};

// 全局变量
const int STAR_COUNT = 350;  // 增加星星数量
Star stars[STAR_COUNT];
DWORD lastUpdateTime = 0;  // 上次更新时间
float animationTime = 0;   // 动画时间累计

StartScene::StartScene(void)
    : m_bgImg(nullptr)
{
    // 尝试加载背景图片
    //m_bgImg = ResManager::instance()->loadImage("Resource/images/starBackground.png");

    // 初始化随机数种子
    srand((unsigned int)time(NULL));

    // 初始化星星
    for (int i = 0; i < STAR_COUNT; i++)
    {
        // 位置完全随机
        stars[i].x = rand() % sApp->width();
        stars[i].y = rand() % sApp->height();

        // 大小随机，使一部分星星更大
        stars[i].size = (rand() % 100 < 85) ? (rand() % 2) + 1 : (rand() % 2) + 2;

        // 亮度基础值 - 确保有足够的亮度变化空间
        stars[i].baseBrightness = 150 + (rand() % 70);

        // 闪烁速度随机（值越小闪烁越慢）
        stars[i].twinkleSpeed = 0.5f + (rand() % 100) / 30.0f;

        // 闪烁相位随机（让每颗星星闪烁周期不同）
        stars[i].twinklePhase = (float)(rand() % 628) / 100.0f; // 0到2π之间

        // 星星颜色随机 - 大多数是白色，但有一些是淡蓝色、淡红色或淡黄色
        int colorRand = rand() % 100;
        if (colorRand < 60)
        {
            // 白色星星
            stars[i].color = RGB(0, 47, 167);
        }
        else if (colorRand < 75)
        {
            // 淡蓝色星星
            stars[i].color = RGB(252, 272, 10);
        }
        else if (colorRand < 95)
        {
            // 淡红色星星
            stars[i].color = RGB(238, 130, 238);
        }
        else if (colorRand < 105)
        {
            // 淡黄色星星
            stars[i].color = RGB(105, 105, 105);
        }
        else
        {
            // 淡红色星星
            stars[i].color = RGB(235, 92, 92);
        }
    }

    // 初始化时间
    lastUpdateTime = GetTickCount();
}

void StartScene::draw()
{
    // 获取屏幕尺寸
    int width = sApp->width();
    int height = sApp->height();

    // 如果背景图片存在，则绘制图片背景
    if (m_bgImg && m_bgImg->getwidth() > 0)
    {
        // 使用项目中已有的drawImage函数绘制背景图片
        drawImage(0, 0, m_bgImg);
    }
    // 否则绘制程序生成的星空背景
    else
    {
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
        for (int i = 0; i < STAR_COUNT; i++)
        {
            // 计算闪烁因子：正弦波形式
            float twinkleFactor = sin(animationTime * stars[i].twinkleSpeed + stars[i].twinklePhase);

            // 将闪烁因子转换为亮度变化（值在-40到40之间）
            int brightnessChange = (int)(twinkleFactor * 40.0f);

            // 计算当前亮度
            int currentBrightness = stars[i].baseBrightness + brightnessChange;
            if (currentBrightness < 100) currentBrightness = 100;
            if (currentBrightness > 255) currentBrightness = 255;

            // 获取基础颜色的RGB值
            int r = GetRValue(stars[i].color);
            int g = GetGValue(stars[i].color);
            int b = GetBValue(stars[i].color);

            // 根据当前亮度调整颜色
            float brightnessFactor = (float)currentBrightness / 255.0f;
            r = (int)(r * brightnessFactor);
            g = (int)(g * brightnessFactor);
            b = (int)(b * brightnessFactor);

            // 绘制星星
            setfillcolor(RGB(r, g, b));

            // 大星星有光晕效果
            if (stars[i].size >= 2)
            {
                // 先画一个稍淡的大圆作为光晕
                setfillcolor(RGB(r / 3, g / 3, b / 3));
                solidcircle(stars[i].x, stars[i].y, stars[i].size + 1);

                // 再画实心圆作为星星主体
                setfillcolor(RGB(r, g, b));
            }

            solidcircle(stars[i].x, stars[i].y, stars[i].size);
        }
    }

    // 绘制UI元素
    drawTitle();
    drawPrompt();
    drawVersion();
}

void StartScene::update()
{
    // 计算时间增量（秒）
    DWORD currentTime = GetTickCount();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    // 更新动画时间
    animationTime += deltaTime;
    
    // 检测鼠标点击
    if (sApp->msg()->message == WM_LBUTTONDOWN)
    {
        sApp->runScene(new GameScene());
    }
}

void StartScene::drawTitle()
{
    // 设置字体
    LOGFONT font;
    MyGetTextStyle(&font);               // 使用自定义函数获取当前字体
    font.lfHeight = 64;                  // 增大字体高度
    font.lfWeight = FW_EXTRABOLD;        // 设置为特粗体(Extra Bold)
    
    // 使用memcpy而不是strncpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // 确保以空字符结尾
    
    MySetTextStyle(&font);               // 使用自定义函数设置字体样式
    setbkmode(TRANSPARENT);            // 设置背景透明

    // 使用与游戏进程中相同的中文字符串方式
    const char* title = "星海弈锋";
    // 使用std::string处理字符串
    std::string titleStr(title);
    
    // 手动计算文本宽度 - 考虑到字体加大和加粗
    int titleWidth = 64 * 4; // 假设4个中文字符，每个宽度为64像素(已增大)
    int titleX = sApp->width() / 2 - titleWidth / 2;
    int titleY = sApp->height() / 2 - 60; // 稍微上移，以适应更大的字体
    
    // 计算基于时间的渐变参数
    float time = animationTime * 1.0f; // 加快渐变速度
    
    // 两个颜色之间渐变 - 使颜色更饱和一些
    int r1 = 100, g1 = 255, b1 = 100; // 更饱和的浅绿色
    int r2 = 100, g2 = 200, b2 = 255; // 更饱和的浅蓝色
    
    // 中文字符处理
    std::wstring wideTitle = AnsiToWide(title);
    int titleLen = static_cast<int>(wideTitle.length());
    
    // 实现描边效果，先绘制黑色底
    for (int i = 0; i < titleLen; i++)
    {
        // 绘制黑色底
        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                if (offsetX != 0 || offsetY != 0) {
                    // 绘制黑色描边
                    MyDrawChar(titleX + i * 64 + offsetX, titleY + offsetY, wideTitle[i], RGB(0, 0, 0));
                }
            }
        }
    }
    
    // 对每个字符分别计算颜色
    for (int i = 0; i < titleLen; i++)
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
        MyDrawChar(titleX + i * 64, titleY, wideTitle[i], RGB(r, g, b)); // 调整字符间距
    }
}

void StartScene::drawPrompt()
{
    // 设置字体
    LOGFONT font;
    MyGetTextStyle(&font);               // 使用自定义函数获取当前字体
    font.lfHeight = 20;                // 设置字体高度
    
    // 使用memcpy而不是strncpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // 确保以空字符结尾
    
    MySetTextStyle(&font);               // 使用自定义函数设置字体样式

    // 使用与游戏进程中相同的中文字符串方式
    const char* prompt = "点击鼠标左键开始游戏";
    
    // 转换为宽字符串
    std::wstring widePrompt = AnsiToWide(prompt);
    
    // 手动计算宽度
    int charCount = static_cast<int>(widePrompt.length());
    int promptWidth = charCount * 20; // 每个字符约24像素宽
    
    // 使用自定义函数绘制文本
    MyDrawText(
        sApp->width() / 2 - promptWidth / 2, 
        sApp->height() / 2 +30, 
        widePrompt.c_str(), 
        RGB(238, 238, 238)
    );
}

void StartScene::drawVersion()
{
    // 设置字体
    LOGFONT font;
    MyGetTextStyle(&font);               // 使用自定义函数获取当前字体
    font.lfHeight = 15;                // 设置字体高度
    
    // 使用memcpy而不是strncpy
    memcpy(font.lfFaceName, "黑体", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // 确保以空字符结尾
    
    MySetTextStyle(&font);               // 使用自定义函数设置字体样式

    // 使用与游戏进程中相同的中文字符串方式
    const char* versionStr = "版本：1.0";
    
    // 转换为宽字符串
    std::wstring wideVersion = AnsiToWide(versionStr);
    
    // 手动计算宽度
    int charCount = static_cast<int>(wideVersion.length());
    int versionWidth = charCount * 15; // 每个字符约16像素宽
    
    // 使用自定义函数绘制文本
    MyDrawText(
        sApp->width() - versionWidth + 10 , 
        sApp->height() - 30, 
        wideVersion.c_str(), 
        RGB(150, 150, 150)
    );
}
