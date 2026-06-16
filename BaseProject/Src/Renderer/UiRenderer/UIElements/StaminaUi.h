#pragma once
#include "../Base/UiBase.h"

class StaminaUi : public UiBase
{
public:
    StaminaUi(float* stamina, float* maxStamina,Vector2 pos);
    ~StaminaUi();

    void Update() override;
    void Draw() override;

private:

    // スタミナ計算
    float* stamina_;
    float* maxStamina_;

    // 表示位置・サイズ
    float cx_;
    float cy_;
    float radius_;

    // 点滅タイマー
    float blinkTimer_;     

    // 30%以下で点滅
    static constexpr float BLINK_THRESHOLD = 0.8f; 
    // 点滅速度
    static constexpr float BLINK_SPEED = 3.0f;   

    // 分割数
    static constexpr int STEPS = 48;
};