#include <DxLib.h>
#include <cmath>
#include "StaminaUi.h"

StaminaUi::StaminaUi(float* stamina, float* maxStamina, Vector2 pos)
    : stamina_(stamina)
    , maxStamina_(maxStamina)
    , UiBase(pos)
    , radius_(30.0f)
    , blinkTimer_(0.0f)
{   
    drawOrder_ = 10;
}

StaminaUi::~StaminaUi()
{
}

void StaminaUi::Update()
{
    float ratio = *stamina_ / *maxStamina_;

    if (ratio <= BLINK_THRESHOLD)
    {
        blinkTimer_ += 1.0f * BLINK_SPEED;
    }
    else
    {
        blinkTimer_ = 0.0f;
    }
}

void StaminaUi::Draw()
{

    if (!stamina_ || !maxStamina_)
    {
        return;
    }

    unsigned int color;
    float cx = pos_.x;
    float cy = pos_.y;

    float ratio = *stamina_ / *maxStamina_;
    if (ratio <= BLINK_THRESHOLD)
    {
        // sinで0?1を行き来させて点滅
        float blink = sinf(blinkTimer_);
        if (blink > 0.0f)
            color = GetColor(255, 0, 0);  // 赤
        else
            color = GetColor(150, 0, 0);  // 暗い赤
    }
    else
    {
        color = GetColor(0, 255, 100);  // 通常の緑
    }

    float angleStep = (2.0f * DX_PI_F) / STEPS;
    int totalSteps = (int)(STEPS * ratio);

    // 背景の円
    DrawCircle((int)cx, (int)cy, (int)radius_,
        GetColor(50, 50, 50), TRUE);

    //// デバッグ表示
    //DrawFormatString(0, 0, GetColor(0, 0, 255),
    //    "stamina:%.1f max:%.1f ratio:%.2f blink:%.2f",
    //    *stamina_, *maxStamina_, ratio, blinkTimer_);

    // スタミナゲージ
    for (int i = 0; i < totalSteps; i++)
    {
        float a1 = angleStep * i - DX_PI_F / 2.0f;
        float a2 = angleStep * (i + 1) - DX_PI_F / 2.0f;

        DrawTriangle(
            (int)cx, (int)cy,
            (int)(cx + cosf(a1) * radius_),
            (int)(cy + sinf(a1) * radius_),
            (int)(cx + cosf(a2) * radius_),
            (int)(cy + sinf(a2) * radius_),
            GetColor(0, 255, 100), TRUE);
    }

    // 外枠の円
    DrawCircle((int)cx, (int)cy, (int)radius_,
        GetColor(255, 255, 255), FALSE);
}