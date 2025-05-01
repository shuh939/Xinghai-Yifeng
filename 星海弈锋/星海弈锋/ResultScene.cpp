#include "ResultScene.h"
#include "GameFrameWork/Application.h"
#include "StartScene.h"
#include <Windows.h>
#include "GameFrameWork/gui/Gui.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "ResManager.h"
#include <graphics.h> // ���ͼ�ο�ͷ�ļ�
#include <string> // ����ַ�������ͷ�ļ�
#include "TimingUtils.h" // ����Զ���ʱ�乤��ͷ�ļ�
#include "TextUtils.h" // ����Զ����ı�����ͷ�ļ�

// �ǿձ������� - ʹ��ȫ�ֱ��������ǰ׺�����ͻ
const int RESULT_STAR_COUNT = 350;  // ��������
ResultStar resultStars[RESULT_STAR_COUNT]; // ʹ��ResultStar����
DWORD resultLastUpdateTime = 0;    // �ϴθ���ʱ��
float resultAnimationTime = 0;     // ����ʱ���ۼ�

// ʹ���Լ���GetTickCount64���ݺ�������������TimingUtils.h

ResultScene::ResultScene(ResultType resultType)
    : m_resultType(resultType)
{
    // ��¼ҳ�洴��ʱ�䣬ʹ�ü��ݺ������GetTickCount64
    m_createTime = MyGetTickCount64();
    
    // ��ʼ�����������
    srand((unsigned int)time(NULL));

    // ��ʼ������
    for (int i = 0; i < RESULT_STAR_COUNT; i++)
    {
        // λ����ȫ���
        resultStars[i].x = rand() % sApp->width();
        resultStars[i].y = rand() % sApp->height();

        // ��С�����ʹһ�������Ǹ���
        resultStars[i].size = (rand() % 100 < 85) ? (rand() % 2) + 1 : (rand() % 2) + 2;

        // ���Ȼ���ֵ - ȷ�����㹻�����ȱ仯�ռ�
        resultStars[i].baseBrightness = 150 + (rand() % 70);

        // ��˸�ٶ������ֵԽС��˸Խ����
        resultStars[i].twinkleSpeed = 0.5f + (rand() % 100) / 30.0f;

        // ��˸��λ�������ÿ��������˸���ڲ�ͬ��
        resultStars[i].twinklePhase = (float)(rand() % 628) / 100.0f; // 0��2��֮��

        // ������ɫ��� - ������ǰ�ɫ������һЩ�ǵ���ɫ������ɫ�򵭻�ɫ
        int colorRand = rand() % 100;
        if (colorRand < 60)
        {
            // ��ɫ����
            resultStars[i].color = RGB(0, 47, 167);
        }
        else if (colorRand < 75)
        {
            // ����ɫ����
            resultStars[i].color = RGB(252, 272, 10);
        }
        else if (colorRand < 95)
        {
            // ����ɫ����
            resultStars[i].color = RGB(238, 130, 238);
        }
        else if (colorRand < 105)
        {
            // ����ɫ����
            resultStars[i].color = RGB(105, 105, 105);
        }
        else
        {
            // ����ɫ����
            resultStars[i].color = RGB(235, 92, 92);
        }
    }

    // ��ʼ��ʱ��
    resultLastUpdateTime = GetTickCount();
}

void ResultScene::draw()
{
    // ��ȡ��Ļ�ߴ�
    int width = sApp->width();
    int height = sApp->height();

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
    for (int i = 0; i < RESULT_STAR_COUNT; i++)
    {
        // ������˸���ӣ����Ҳ���ʽ
        float twinkleFactor = sin(resultAnimationTime * resultStars[i].twinkleSpeed + resultStars[i].twinklePhase);

        // ����˸����ת��Ϊ���ȱ仯��ֵ��-40��40֮�䣩
        int brightnessChange = (int)(twinkleFactor * 40.0f);

        // ���㵱ǰ����
        int currentBrightness = resultStars[i].baseBrightness + brightnessChange;
        if (currentBrightness < 100) currentBrightness = 100;
        if (currentBrightness > 255) currentBrightness = 255;

        // ��ȡ������ɫ��RGBֵ
        int r = GetRValue(resultStars[i].color);
        int g = GetGValue(resultStars[i].color);
        int b = GetBValue(resultStars[i].color);

        // ���ݵ�ǰ���ȵ�����ɫ
        float brightnessFactor = (float)currentBrightness / 255.0f;
        r = (int)(r * brightnessFactor);
        g = (int)(g * brightnessFactor);
        b = (int)(b * brightnessFactor);

        // ��������
        setfillcolor(RGB(r, g, b));

        // �������й���Ч��
        if (resultStars[i].size >= 2)
        {
            // �Ȼ�һ���Ե��Ĵ�Բ��Ϊ����
            setfillcolor(RGB(r / 3, g / 3, b / 3));
            solidcircle(resultStars[i].x, resultStars[i].y, resultStars[i].size + 1);

            // �ٻ�ʵ��Բ��Ϊ��������
            setfillcolor(RGB(r, g, b));
        }

        solidcircle(resultStars[i].x, resultStars[i].y, resultStars[i].size);
    }

    // ����UIԪ�� - ֱ�ӻ��ƽ����ı�����ʹ��GUI_Label
    
    // ��ʾ"YOU WIN!"�ı���������Ч����
    const char* winText = "YOU WIN!";
    LOGFONT font;
    MyGetTextStyle(&font);           // ʹ���Զ��庯������gettextstyle
    font.lfHeight = 64;              // ��������߶�
    font.lfWeight = FW_EXTRABOLD;    // ����Ϊ�ش���(Extra Bold)
    
    // �޸�strncpy���棬ʹ��memcpy
    memcpy(font.lfFaceName, "����", sizeof(TCHAR) * 2);
    font.lfFaceName[2] = '\0';       // ȷ���Կ��ַ���β
    
    MySetTextStyle(&font);           // ʹ���Զ��庯������settextstyle
    setbkmode(TRANSPARENT);          // ���ñ���͸��
    
    // ʹ�ñ�׼�ַ���������
    std::string winTextStr(winText);
    int winTextLen = static_cast<int>(winTextStr.length());
    
    // �ֶ������ı���ȶ���ʹ��textwidth - ���ǵ�����Ӵ�ͼӴ�
    int textWidth = 0;
    for (int i = 0; i < winTextLen; i++) {
        // Ϊ��������������ÿ���ַ����Ϊ32����
        // ʵ��Ӧ���п�����Ҫ����ȷ�ļ��㷽��
        textWidth += 32;
    }
    
    int textX = sApp->width() / 2 - textWidth / 2;
    int textY = sApp->height() / 2 - 60; // ��΢���ƣ�����Ӧ���������
    
    // �������ʱ��Ľ������
    float time = resultAnimationTime * 1.0f; // �ӿ콥���ٶ�
    
    // ������ɫ֮�佥�� - ʹ��ɫ������һЩ
    int r1 = 100, g1 = 255, b1 = 100; // �����͵�ǳ��ɫ
    int r2 = 100, g2 = 200, b2 = 255; // �����͵�ǳ��ɫ
    
    // ʵ�����Ч�����Ȼ��ƺ�ɫ��
    int currentX = textX;
    for (int i = 0; i < winTextLen; i++)
    {
        // ���ƺ�ɫ��
        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                if (offsetX != 0 || offsetY != 0) {
                    // ���ƺ�ɫ���
                    MyDrawChar(currentX + offsetX, textY + offsetY, (wchar_t)winTextStr[i], RGB(0, 0, 0));
                }
            }
        }
        
        // λ�õ���
        currentX += 32;
    }
    
    // ��ÿ���ַ��ֱ������ɫ��λ��
    currentX = textX;
    for (int i = 0; i < winTextLen; i++)
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
        MyDrawChar(currentX, textY, (wchar_t)winTextStr[i], RGB(r, g, b));
        
        // ÿ���ַ����ԼΪ32���� (��Ϊ����Ӵ���)
        currentX += 32;
    }
    
    // ��ʾ��ʾ�ı�
    font.lfHeight = 24;
    MySetTextStyle(&font);
    
    const char* promptText = "������������ؿ�ʼҳ��";
    
    // ����ʹ��textwidth
    int promptWidth = 24 * strlen(promptText) / 2; // �����ַ�ռ2�ֽڣ�Ӣ���ַ�ռ1�ֽ�
    
    // ת��Ϊ���ַ���
    std::wstring widePrompt = AnsiToWide(promptText);
    
    // ʹ���Զ��庯�������ı�
    MyDrawText(
        sApp->width() / 2 - promptWidth / 2, 
        sApp->height() / 2 + 20, 
        widePrompt.c_str(), 
        RGB(200, 200, 200)
    );
    
    // ��ʾ�汾��
    font.lfHeight = 16;
    MySetTextStyle(&font);
    
    const char* versionText = "�汾��1.0";
    
    // ����ʹ��textwidth
    int versionWidth = 16 * strlen(versionText) / 2; // �����ַ�ռ2�ֽ�
    
    // ת��Ϊ���ַ���
    std::wstring wideVersion = AnsiToWide(versionText);
    
    // ʹ���Զ��庯�������ı�
    MyDrawText(
        sApp->width() - versionWidth - 20, 
        sApp->height() - 30, 
        wideVersion.c_str(), 
        RGB(150, 150, 150)
    );
}

void ResultScene::update()
{
    // ����ʱ���������룩
    DWORD currentTime = GetTickCount();
    float deltaTime = (currentTime - resultLastUpdateTime) / 1000.0f;
    resultLastUpdateTime = currentTime;

    // ���¶���ʱ��
    resultAnimationTime += deltaTime;
    
    // ������Ҫ�����������ֽ���Ч������Ϊ����ʹ�ö���ʱ��
    
    // ������̫��������������ҳ�棬ʹ�ü��ݺ������GetTickCount64
    if (MyGetTickCount64() - m_createTime < 500)
    {
        return;
    }

    // ��������������󷵻ؿ�ʼҳ��
    if (sApp->msg()->message == WM_LBUTTONDOWN)
    {
        sApp->runScene(new StartScene());
    }
}
