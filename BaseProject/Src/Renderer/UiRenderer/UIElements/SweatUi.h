#pragma once
#include "../Base/UIBillboardBase.h"

class SweatUI : public UIBillboardBase
{
public:

    SweatUI(VECTOR pos);
    ~SweatUI();
    void Update() override;
    void Draw() override;

private:

};