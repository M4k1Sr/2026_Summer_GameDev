#pragma once
#include <DxLib.h>

class UIBillboardBase
{
public:

    UIBillboardBase(VECTOR pos);
    virtual ~UIBillboardBase();

    virtual void Update() = 0;
    virtual void Draw() = 0;

    int drawOrder_;

protected:
    
    int drawUIH_;   // UIBillboard画像モデル
    VECTOR pos_;    // 座標
    VECTOR scl_;    // スケール
    VECTOR rot_;    // 角度
    float timer_;   // 表示時間
    bool isVisible_;// 点滅するか

};
