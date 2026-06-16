#pragma once
#include <vector>
#include <algorithm>
#include "../Base/UiBase.h"
#include "../Base/UiBillboardBase.h"
#include "../../EffectRenderer/Base/EffectBase.h"

class UIManager
{
public:

    UIManager();
    ~UIManager();
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