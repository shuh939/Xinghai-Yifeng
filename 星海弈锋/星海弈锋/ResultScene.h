#pragma once
#include "GameFrameWork/Scene.h"
#include <Windows.h> // Windows.h头文件以定义ULONGLONG类型

// 结算页面专用星星结构体
struct ResultStar
{
    int x, y;              // 位置
    int size;              // 大小
    int baseBrightness;    // 基础亮度
    float twinkleSpeed;    // 闪烁速度
    float twinklePhase;    // 闪烁相位
    COLORREF color;        // 星星颜色
};

// 声明外部变量，在cpp文件中定义
extern const int RESULT_STAR_COUNT;
extern ResultStar resultStars[];
extern DWORD resultLastUpdateTime;
extern float resultAnimationTime;

class ResultScene : public Scene
{
public:
    // 游戏结果类型
    enum ResultType
    {
        RedWin,  // 红方获胜
        BlackWin // 黑方获胜
    };

    // 构造函数接收游戏结果类型
    ResultScene(ResultType resultType);
    // 不需要显式声明析构函数，使用默认析构函数
    void draw() override;
    void update() override;

private:
    ResultType m_resultType; // 保存游戏结果
    ULONGLONG m_createTime;  // 记录页面创建时间
};
