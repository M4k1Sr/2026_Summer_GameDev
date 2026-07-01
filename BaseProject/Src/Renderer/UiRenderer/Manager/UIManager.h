#pragma once
#include <vector>
#include <algorithm>
#include "../Base/UIBase.h"
#include "../Base/UIBillboardBase.h"

class UIManager
{
public:

    UIManager();
    ~UIManager();
    void AddUIBase(UIBase* UI);
    void AddUIBillboardBase(UIBillboardBase* UIBillboard);
    void Update();
    void Draw();
    void Release();

private:
    std::vector<UIBase*> UIList_;

    std::vector<UIBillboardBase*> UIBillboardList_;

};