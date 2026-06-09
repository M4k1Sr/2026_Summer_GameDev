#pragma once
#include <vector>
#include <algorithm>
#include "../Renderer/UiRenderer/UiBase.h"

class DrawableManager
{
public:
    DrawableManager();
    ~DrawableManager();

    void Add(UiBase* ui);
    void Update();
    void Draw();
    void Release();

private:
    std::vector<UiBase*> uiList_;
};