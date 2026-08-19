#pragma once
#include "../Base/UIBase.h"

class GimmickUi : public UIBase
{
public:
    GimmickUi(float* gimmick, float* maxGimmick, Vector2 pos);
    ~GimmickUi();

    void Update() override;
    void Draw() override;

private:

    // ギミック計算
    float* gimmick_;
    float* maxGimmick_;

    // ギミック値保持変数
    float oldGimmick_;

    // 表示タイマー
    float showTimer_ = 0.0f;

    // 表示位置・サイズ
    float cx_;
    float cy_;
    float radius_;

    // 点滅タイマー
    float blinkTimer_;

    // ギミック変化フラグ
    bool isChangeGimmick_;

    // 30%以下で点滅
    static constexpr float BLINK_THRESHOLD = 0.8f;

    // 表示時間
    static constexpr float SHOW_DURATION = 90.0f;

    // 点滅速度
    static constexpr float BLINK_SPEED = 3.0f;

    // 分割数
    static constexpr int STEPS = 48;
};