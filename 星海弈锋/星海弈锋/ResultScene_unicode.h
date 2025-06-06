#pragma once
#include "GameFrameWork/Scene.h"
#include <Windows.h> // 添加Windows.h头文件以定义ULONGLONG类型

class ResultScene : public Scene
{
public:
    // 使用枚举表示游戏结果类型
    enum ResultType
    {
        RedWin,  // 红方获胜
        BlackWin // 黑方获胜
    };

    // 构造函数接收游戏结果类型
    ResultScene(ResultType resultType);
    void draw() override;
    void update() override;

private:
    ResultType m_resultType; // 保存游戏结果
    ULONGLONG m_createTime;  // 记录页面创建时间，使用ULONGLONG类型适应GetTickCount64
}; 
