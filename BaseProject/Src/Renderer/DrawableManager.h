#pragma once
#include <vector>
#include <algorithm>
#include "../Renderer/UiRenderer/UiBase.h"
#include "../Renderer/UiRenderer/UiBillboardBase.h"
#include"../Renderer/EffectRenderer/EffectBase.h"

class DrawableManager
{
public:
    DrawableManager();
    ~DrawableManager();

    void PlayEffect(int effectHandle, VECTOR pos,VECTOR scale);

    void AddUiBase(UiBase* ui);
    void AddUiBillboardBase(UiBillboardBase* uiBillboard);
    void Update();
    void Draw();
    void Release();

private:
    std::vector<UiBase*> uiList_;

    std::vector<UiBillboardBase*> uiBillboardList_;

    std::vector< std::unique_ptr<EffectBase> > effects_;
};