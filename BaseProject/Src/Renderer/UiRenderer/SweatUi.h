#pragma once
#include "UiBase.h"

class SweatUi : public UiBase
{
public:
    SweatUi(float* sweat, float* maxSweat);
    ~SweatUi();
    void Update() override;
    void Draw() override;

private:

    

    // 表示位置・サイズ
    float cx_;
    float cy_;
    float radius_;

    // 点滅タイマー
    float blinkTimer_;

    // 30%以下で点滅
    static constexpr float BLINK_THRESHOLD = 0.3f;
    // 点滅速度
    static constexpr float BLINK_SPEED = 3.0f;

    // 分割数
    static constexpr int STEPS = 48;
};