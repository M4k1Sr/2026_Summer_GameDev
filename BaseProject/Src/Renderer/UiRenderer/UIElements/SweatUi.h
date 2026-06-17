#pragma once
#include "../Base/UiBillboardBase.h"

class SweatUi : public UiBillboardBase
{
public:

    SweatUi(VECTOR pos);
    ~SweatUi();
    void Update() override;
    void Draw() override;

private:

};