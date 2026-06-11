#pragma once
#include <vector>
#include <algorithm>
#include "../Renderer/UiRenderer/UiBase.h"
#include "../Renderer/UiRenderer/UiBillboardBase.h"

class DrawableManager
{
public:
    DrawableManager();
    ~DrawableManager();

    void AddUiBase(UiBase* ui);
    void AddUiBillboardBase(UiBillboardBase* uiBillboard);
    void Update();
    void Draw();
    void Release();

private:
    std::vector<UiBase*> uiList_;

    std::vector<UiBillboardBase*> uiBillboardList_;

};