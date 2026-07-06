#pragma once
#include "../Base/UIBase.h"


class StaminaUI : public UIBase
{
public:
    StaminaUI(float* stamina, float* maxStamina,Vector2 pos);
    ~StaminaUI();

    void Update() override;
    void Draw() override;

private:

    // スタミナ計算
    float* stamina_;
    float* maxStamina_;
    //プレイヤーのダッシュ判定
	bool* isDash_;

    // スタミナ値保持変数
    float oldStamina_;

	// 表示タイマー
    float showTimer_ = 0.0f;

    // 表示位置・サイズ
    float cx_;
    float cy_;
    float radius_;

    // 点滅タイマー
    float blinkTimer_;     

	// スタミナ変化フラグ
    bool isChangeStamina_;

    // 30%以下で点滅
    static constexpr float BLINK_THRESHOLD = 0.8f; 

    // 表示時間
    static constexpr float SHOW_DURATION = 90.0f;

    // 点滅速度
    static constexpr float BLINK_SPEED = 3.0f;   

    // 分割数
    static constexpr int STEPS = 48;
};