#pragma once
#include <DxLib.h>

struct DrawBillboardUi
{
    int drawUiH_;   // Ui画像モデル
    VECTOR pos_;    // 座標
    VECTOR scl_;    // スケール
    VECTOR rot_;    // 角度
    float timer_;   // 表示時間
    bool isVisible_;// 点滅するか
};

class UiBillboardBase
{
public:

    UiBillboardBase();
    UiBillboardBase(const DrawBillboardUi& uiData);
    virtual ~UiBillboardBase();

    virtual void Update() = 0;
    virtual void Draw() = 0;

    int drawOrder_;

protected:

    DrawBillboardUi* uiBillboard_;

};
