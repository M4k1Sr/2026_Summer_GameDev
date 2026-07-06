#include <DxLib.h>
#include <cmath>
#include "StaminaUI.h"

StaminaUI::StaminaUI(float* stamina, float* maxStamina, Vector2 pos)
    : stamina_(stamina)
    , maxStamina_(maxStamina)
    , UIBase(pos)
    , radius_(30.0f)
    , blinkTimer_(0.0f) 
    , isChangeStamina_(false)
{   
    drawOrder_ = 10;
}

StaminaUI::~StaminaUI()
{
}

void StaminaUI::Update()
{
    if (!stamina_ || !maxStamina_)
    {
        return;
	}

    static bool isFirstFrame = true;
    if (isFirstFrame)
    {
		oldStamina_ = *stamina_;
		isFirstFrame = false;
        return;
	}

    // 1. スタミナに変化があったかチェック
    if (*stamina_ != oldStamina_)
    {
        showTimer_ = SHOW_DURATION; // 変化していたらタイマーを最大まで回復
    }
    else if (showTimer_ > 0.0f)
    {
        showTimer_ -= 1.0f; // 変化が止まっていたら毎フレーム減算
    }

    float ratio = *stamina_ / *maxStamina_;
    if (ratio <= BLINK_THRESHOLD)
    {
        blinkTimer_ += 1.0f * BLINK_SPEED;
    }
    else
    {
        blinkTimer_ = 0.0f;
    }

    // スタミナの値を保持
    oldStamina_ = *stamina_;

}

void StaminaUI::Draw()
{
<<<<<<< HEAD
  

=======
>>>>>>> m4k
    if (!stamina_ || !maxStamina_)
    {
        return;
    }

	// スタミナが最大値の場合は描画しない
    if (*stamina_ >= *maxStamina_)
    {
        return;
    }

    unsigned int color;
    float cx = pos_.x;
    float cy = pos_.y;

    float ratio = *stamina_ / *maxStamina_;
	isChangeStamina_ = (showTimer_ > 0.0f) || (ratio <= BLINK_THRESHOLD);
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

    if (isChangeStamina_)
    {
        // スタミナが変化した場合の処理
        // 背景の円
        DrawCircle((int)cx, (int)cy, (int)radius_,
            GetColor(50, 50, 50), TRUE);


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

    if (stamina_ == maxStamina_)
    {
        // スタミナが最大値の時は、ゲージを表示しない
		isChangeStamina_ = false;
        return;
	}
}