#pragma once
#include <DxLib.h>

class UiBillboardBase
{
public:

    UiBillboardBase(VECTOR pos);
    virtual ~UiBillboardBase();

    virtual void Update() = 0;
    virtual void Draw() = 0;

    int drawOrder_;

protected:

    int drawUiH_;   // UiBillboard画像モデル
    VECTOR pos_;    // 座標
    VECTOR scl_;    // スケール
    VECTOR rot_;    // 角度
    float timer_;   // 表示時間
    bool isVisible_;// 点滅するか

};
