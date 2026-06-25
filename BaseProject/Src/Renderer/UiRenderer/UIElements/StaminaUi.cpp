#include <DxLib.h>
#include <cmath>
#include "StaminaUI.h"

StaminaUI::StaminaUI(float* stamina, float* maxStamina, Vector2 pos)
    : stamina_(stamina)
    , maxStamina_(maxStamina)
    , UIBase(pos)
    , radius_(30.0f)
    , blinkTimer_(0.0f)
{   
    drawOrder_ = 10;
}

StaminaUI::~StaminaUI()
{
}

void StaminaUI::Update()
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

void StaminaUI::Draw()
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
        // sinÇ≈0?1ÇçsÇ´óàÇ≥ÇπÇƒì_ñ≈
        float blink = sinf(blinkTimer_);
        if (blink > 0.0f)
            color = GetColor(255, 0, 0);  // ê‘
        else
            color = GetColor(150, 0, 0);  // à√Ç¢ê‘
    }
    else
    {
        color = GetColor(0, 255, 100);  // í èÌÇÃóŒ
    }

    float angleStep = (2.0f * DX_PI_F) / STEPS;
    int totalSteps = (int)(STEPS * ratio);

    // îwåiÇÃâ~
    DrawCircle((int)cx, (int)cy, (int)radius_,
        GetColor(50, 50, 50), TRUE);


    // ÉXÉ^É~ÉiÉQÅ[ÉW
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

    // äOògÇÃâ~
    DrawCircle((int)cx, (int)cy, (int)radius_,
        GetColor(255, 255, 255), FALSE);
}