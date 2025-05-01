#pragma once
#include "GameFrameWork/Scene.h"

class StartScene : public Scene
{
public:
    StartScene(void);
    void draw() override;
    void update() override;
private:
    // Helper functions for drawing text
    void drawTitle();
    void drawPrompt();
    void drawVersion();

    IMAGE* m_bgImg{}; // ±³¾°Í¼Æ¬
};
