#pragma once
#include <DxLib.h>
#include "../../../Common/Vector2.h"

class UIBase {
public:
    UIBase(Vector2 pos);
    virtual ~UIBase ();
    virtual void Update() = 0;
    virtual void Draw() = 0;
    int drawOrder_;

protected:

    int drawUIH_;   // UI画像モデル
    Vector2 pos_;    // 座標
    Vector2 scl_;    // スケール
    Vector2 rot_;    // 角度
    float timer_;   // 表示時間
    bool isVisible_;// 点滅するか

};