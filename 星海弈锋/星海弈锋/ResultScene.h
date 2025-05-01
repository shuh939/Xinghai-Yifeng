#pragma once
#include "GameFrameWork/Scene.h"
#include <Windows.h> // Windows.hͷ�ļ��Զ���ULONGLONG����

// ����ҳ��ר�����ǽṹ��
struct ResultStar
{
    int x, y;              // λ��
    int size;              // ��С
    int baseBrightness;    // ��������
    float twinkleSpeed;    // ��˸�ٶ�
    float twinklePhase;    // ��˸��λ
    COLORREF color;        // ������ɫ
};

// �����ⲿ��������cpp�ļ��ж���
extern const int RESULT_STAR_COUNT;
extern ResultStar resultStars[];
extern DWORD resultLastUpdateTime;
extern float resultAnimationTime;

class ResultScene : public Scene
{
public:
    // ��Ϸ�������
    enum ResultType
    {
        RedWin,  // �췽��ʤ
        BlackWin // �ڷ���ʤ
    };

    // ���캯��������Ϸ�������
    ResultScene(ResultType resultType);
    // ����Ҫ��ʽ��������������ʹ��Ĭ����������
    void draw() override;
    void update() override;

private:
    ResultType m_resultType; // ������Ϸ���
    ULONGLONG m_createTime;  // ��¼ҳ�洴��ʱ��
};
