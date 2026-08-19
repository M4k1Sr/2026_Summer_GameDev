#include <DxLib.h>
#include <cmath>
#include "GimmickUi.h"

GimmickUi::GimmickUi(float* gimmick, float* maxGimmick, Vector2 pos)
    : gimmick_(gimmick)
    , maxGimmick_(maxGimmick)
    , UIBase(pos)
    , radius_(30.0f)
    , blinkTimer_(0.0f)
    , isChangeGimmick_(false)
{
    drawOrder_ = 10;
}

GimmickUi::~GimmickUi()
{
}

void GimmickUi::Update()
{
    if (!gimmick_ || !maxGimmick_)
    {
        return;
    }

    if (*gimmick_ <= 0.0f)
    {
        // ギミックゲージが0以下の時は表示しない
        isChangeGimmick_ = false;
        return;
    }
    else {
        // ギミックゲージが0以下の時は表示
        isChangeGimmick_ = true;
        return;

    }

    static bool isFirstFrame = true;
    if (isFirstFrame)
    {
        oldGimmick_ = *gimmick_;
        isFirstFrame = false;
        return;
    }

    // 1. ギミックゲージに変化があったかチェック
    if (*gimmick_ != oldGimmick_)
    {
        showTimer_ = SHOW_DURATION; // 変化していたらタイマーを最大まで回復
    }
    else if (showTimer_ > 0.0f)
    {
        showTimer_ -= 1.0f; // 変化が止まっていたら毎フレーム減算
    }

    float ratio = *gimmick_ / *maxGimmick_;
    if (ratio <= BLINK_THRESHOLD)
    {
        blinkTimer_ += 1.0f * BLINK_SPEED;
    }
    else
    {
        blinkTimer_ = 0.0f;
    }

    // ギミックゲージの値を保持
    oldGimmick_ = *gimmick_;

}

void GimmickUi::Draw()
{
    if (!gimmick_ || !maxGimmick_)
    {
        return;
    }

    unsigned int color;
    float cx = pos_.x;
    float cy = pos_.y;

    float ratio = *gimmick_ / *maxGimmick_;
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
        color = GetColor(0, 128, 128);  // 通常の緑
    }

    float angleStep = (2.0f * DX_PI_F) / STEPS;
    int totalSteps = (int)(STEPS * ratio);

    if (isChangeGimmick_)
    {
        // ギミックゲージが変化した場合の処理
        // 背景の円
        DrawCircle((int)cx, (int)cy, (int)radius_,
            GetColor(50, 50, 50), TRUE);


        // ギミックゲージ
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
                GetColor(255, 255, 0), TRUE);
        }

        // 外枠の円
        DrawCircle((int)cx, (int)cy, (int)radius_,
            GetColor(255, 255, 255), FALSE);

    }


}