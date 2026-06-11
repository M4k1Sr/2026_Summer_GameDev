#pragma once
#include <DxLib.h>
#include "../../Common/Vector2.h"

class UiBase {
public:
    UiBase(Vector2 pos);
    virtual ~UiBase();
    virtual void Update() = 0;
    virtual void Draw() = 0;
    int drawOrder_;

protected:

    int drawUiH_;   // Ui画像モデル
    Vector2 pos_;    // 座標
    Vector2 scl_;    // スケール
    Vector2 rot_;    // 角度
    float timer_;   // 表示時間
    bool isVisible_;// 点滅するか

};