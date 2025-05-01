#include "StartScene.h"
#include "GameFrameWork/Application.h"
#include "GameScene.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <graphics.h>
#include <math.h>
#include "ResManager.h"
#include <string> // ����ַ�������ͷ�ļ�
#include "TimingUtils.h" // ���ͨ��ʱ�乤��ͷ�ļ�
#include "TextUtils.h" // ����Զ����ı�����ͷ�ļ�

// ���ǽṹ��
struct Star
{
    int x, y;              // λ��
    int size;              // ��С
    int baseBrightness;    // ��������
    float twinkleSpeed;    // ��˸�ٶ�
    float twinklePhase;    // ��˸��λ
    COLORREF color;        // ������ɫ
};

// ȫ�ֱ���
const int STAR_COUNT = 350;  // ������������
Star stars[STAR_COUNT];
DWORD lastUpdateTime = 0;  // �ϴθ���ʱ��
float animationTime = 0;   // ����ʱ���ۼ�

StartScene::StartScene(void)
    : m_bgImg(nullptr)
{
    // ���Լ��ر���ͼƬ
    //m_bgImg = ResManager::instance()->loadImage("Resource/images/starBackground.png");

    // ��ʼ�����������
    srand((unsigned int)time(NULL));

    // ��ʼ������
    for (int i = 0; i < STAR_COUNT; i++)
    {
        // λ����ȫ���
        stars[i].x = rand() % sApp->width();
        stars[i].y = rand() % sApp->height();

        // ��С�����ʹһ�������Ǹ���
        stars[i].size = (rand() % 100 < 85) ? (rand() % 2) + 1 : (rand() % 2) + 2;

        // ���Ȼ���ֵ - ȷ�����㹻�����ȱ仯�ռ�
        stars[i].baseBrightness = 150 + (rand() % 70);

        // ��˸�ٶ������ֵԽС��˸Խ����
        stars[i].twinkleSpeed = 0.5f + (rand() % 100) / 30.0f;

        // ��˸��λ�������ÿ��������˸���ڲ�ͬ��
        stars[i].twinklePhase = (float)(rand() % 628) / 100.0f; // 0��2��֮��

        // ������ɫ��� - ������ǰ�ɫ������һЩ�ǵ���ɫ������ɫ�򵭻�ɫ
        int colorRand = rand() % 100;
        if (colorRand < 60)
        {
            // ��ɫ����
            stars[i].color = RGB(0, 47, 167);
        }
        else if (colorRand < 75)
        {
            // ����ɫ����
            stars[i].color = RGB(252, 272, 10);
        }
        else if (colorRand < 95)
        {
            // ����ɫ����
            stars[i].color = RGB(238, 130, 238);
        }
        else if (colorRand < 105)
        {
            // ����ɫ����
            stars[i].color = RGB(105, 105, 105);
        }
        else
        {
            // ����ɫ����
            stars[i].color = RGB(235, 92, 92);
        }
    }

    // ��ʼ��ʱ��
    lastUpdateTime = GetTickCount();
}

void StartScene::draw()
{
    // ��ȡ��Ļ�ߴ�
    int width = sApp->width();
    int height = sApp->height();

    // �������ͼƬ���ڣ������ͼƬ����
    if (m_bgImg && m_bgImg->getwidth() > 0)
    {
        // ʹ����Ŀ�����е�drawImage�������Ʊ���ͼƬ
        drawImage(0, 0, m_bgImg);
    }
    // ������Ƴ������ɵ��ǿձ���
    else
    {
        // ������ɫ���� - ����������ɫ������������΢�仯
        for (int y = 0; y < height; y += 2) // ÿ���л���һ�Σ��������
        {
            float ratio = (float)y / height;
            // ������ɫ���������ӽ���ɫ
            int r = 2 + (int)(5 * ratio);
            int g = 2 + (int)(5 * ratio);
            int b = 10 + (int)(20 * ratio);

            // �ڻ�����ɫ����������㣬ģ���Ǽʳ���
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
                line(0, y + 1, width, y + 1); // �����һ�У��������
            }
        }

        // ����Զ����Ⱥ - С���ܼ����ǵ㣬����˸
        for (int i = 0; i < 300; i++)
        {
            int x = (i * 29) % width;
            int y = (i * 37) % height;
            int brightness = 50 + (rand() % 50);

            putpixel(x, y, RGB(brightness, brightness, brightness));
        }

        // ������˸������
        for (int i = 0; i < STAR_COUNT; i++)
        {
            // ������˸���ӣ����Ҳ���ʽ
            float twinkleFactor = sin(animationTime * stars[i].twinkleSpeed + stars[i].twinklePhase);

            // ����˸����ת��Ϊ���ȱ仯��ֵ��-40��40֮�䣩
            int brightnessChange = (int)(twinkleFactor * 40.0f);

            // ���㵱ǰ����
            int currentBrightness = stars[i].baseBrightness + brightnessChange;
            if (currentBrightness < 100) currentBrightness = 100;
            if (currentBrightness > 255) currentBrightness = 255;

            // ��ȡ������ɫ��RGBֵ
            int r = GetRValue(stars[i].color);
            int g = GetGValue(stars[i].color);
            int b = GetBValue(stars[i].color);

            // ���ݵ�ǰ���ȵ�����ɫ
            float brightnessFactor = (float)currentBrightness / 255.0f;
            r = (int)(r * brightnessFactor);
            g = (int)(g * brightnessFactor);
            b = (int)(b * brightnessFactor);

            // ��������
            setfillcolor(RGB(r, g, b));

            // �������й���Ч��
            if (stars[i].size >= 2)
            {
                // �Ȼ�һ���Ե��Ĵ�Բ��Ϊ����
                setfillcolor(RGB(r / 3, g / 3, b / 3));
                solidcircle(stars[i].x, stars[i].y, stars[i].size + 1);

                // �ٻ�ʵ��Բ��Ϊ��������
                setfillcolor(RGB(r, g, b));
            }

            solidcircle(stars[i].x, stars[i].y, stars[i].size);
        }
    }

    // ����UIԪ��
    drawTitle();
    drawPrompt();
    drawVersion();
}

void StartScene::update()
{
    // ����ʱ���������룩
    DWORD currentTime = GetTickCount();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    // ���¶���ʱ��
    animationTime += deltaTime;
    
    // ��������
    if (sApp->msg()->message == WM_LBUTTONDOWN)
    {
        sApp->runScene(new GameScene());
    }
}

void StartScene::drawTitle()
{
    // ��������
    LOGFONT font;
    MyGetTextStyle(&font);               // ʹ���Զ��庯����ȡ��ǰ����
    font.lfHeight = 64;                  // ��������߶�
    font.lfWeight = FW_EXTRABOLD;        // ����Ϊ�ش���(Extra Bold)
    
    // ʹ��memcpy������strncpy
    memcpy(font.lfFaceName, "����", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // ȷ���Կ��ַ���β
    
    MySetTextStyle(&font);               // ʹ���Զ��庯������������ʽ
    setbkmode(TRANSPARENT);            // ���ñ���͸��

    // ʹ������Ϸ��������ͬ�������ַ�����ʽ
    const char* title = "�Ǻ��ķ�";
    // ʹ��std::string�����ַ���
    std::string titleStr(title);
    
    // �ֶ������ı���� - ���ǵ�����Ӵ�ͼӴ�
    int titleWidth = 64 * 4; // ����4�������ַ���ÿ�����Ϊ64����(������)
    int titleX = sApp->width() / 2 - titleWidth / 2;
    int titleY = sApp->height() / 2 - 60; // ��΢���ƣ�����Ӧ���������
    
    // �������ʱ��Ľ������
    float time = animationTime * 1.0f; // �ӿ콥���ٶ�
    
    // ������ɫ֮�佥�� - ʹ��ɫ������һЩ
    int r1 = 100, g1 = 255, b1 = 100; // �����͵�ǳ��ɫ
    int r2 = 100, g2 = 200, b2 = 255; // �����͵�ǳ��ɫ
    
    // �����ַ�����
    std::wstring wideTitle = AnsiToWide(title);
    int titleLen = static_cast<int>(wideTitle.length());
    
    // ʵ�����Ч�����Ȼ��ƺ�ɫ��
    for (int i = 0; i < titleLen; i++)
    {
        // ���ƺ�ɫ��
        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                if (offsetX != 0 || offsetY != 0) {
                    // ���ƺ�ɫ���
                    MyDrawChar(titleX + i * 64 + offsetX, titleY + offsetY, wideTitle[i], RGB(0, 0, 0));
                }
            }
        }
    }
    
    // ��ÿ���ַ��ֱ������ɫ
    for (int i = 0; i < titleLen; i++)
    {
        // ����ÿ���ַ�����λƫ�ƣ��������˽���Ч��
        float phase = time + i * 0.8f; // ������λ�ʹ����������
        
        // ʹ�����Ҳ���������ɫ֮��ƽ������
        float ratio = (sin(phase) + 1.0f) * 0.5f; // ������ֵ(-1��1)ӳ�䵽0��1
        
        // ���㵱ǰ�ַ�����ɫ
        int r = (int)(r1 + (r2 - r1) * ratio);
        int g = (int)(g1 + (g2 - g1) * ratio);
        int b = (int)(b1 + (b2 - b1) * ratio);
        
        // ʹ���Զ��庯�����Ƶ����ַ�
        MyDrawChar(titleX + i * 64, titleY, wideTitle[i], RGB(r, g, b)); // �����ַ����
    }
}

void StartScene::drawPrompt()
{
    // ��������
    LOGFONT font;
    MyGetTextStyle(&font);               // ʹ���Զ��庯����ȡ��ǰ����
    font.lfHeight = 20;                // ��������߶�
    
    // ʹ��memcpy������strncpy
    memcpy(font.lfFaceName, "����", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // ȷ���Կ��ַ���β
    
    MySetTextStyle(&font);               // ʹ���Զ��庯������������ʽ

    // ʹ������Ϸ��������ͬ�������ַ�����ʽ
    const char* prompt = "�����������ʼ��Ϸ";
    
    // ת��Ϊ���ַ���
    std::wstring widePrompt = AnsiToWide(prompt);
    
    // �ֶ�������
    int charCount = static_cast<int>(widePrompt.length());
    int promptWidth = charCount * 20; // ÿ���ַ�Լ24���ؿ�
    
    // ʹ���Զ��庯�������ı�
    MyDrawText(
        sApp->width() / 2 - promptWidth / 2, 
        sApp->height() / 2 +30, 
        widePrompt.c_str(), 
        RGB(238, 238, 238)
    );
}

void StartScene::drawVersion()
{
    // ��������
    LOGFONT font;
    MyGetTextStyle(&font);               // ʹ���Զ��庯����ȡ��ǰ����
    font.lfHeight = 15;                // ��������߶�
    
    // ʹ��memcpy������strncpy
    memcpy(font.lfFaceName, "����", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';        // ȷ���Կ��ַ���β
    
    MySetTextStyle(&font);               // ʹ���Զ��庯������������ʽ

    // ʹ������Ϸ��������ͬ�������ַ�����ʽ
    const char* versionStr = "�汾��1.0";
    
    // ת��Ϊ���ַ���
    std::wstring wideVersion = AnsiToWide(versionStr);
    
    // �ֶ�������
    int charCount = static_cast<int>(wideVersion.length());
    int versionWidth = charCount * 15; // ÿ���ַ�Լ16���ؿ�
    
    // ʹ���Զ��庯�������ı�
    MyDrawText(
        sApp->width() - versionWidth + 10 , 
        sApp->height() - 30, 
        wideVersion.c_str(), 
        RGB(150, 150, 150)
    );
}
