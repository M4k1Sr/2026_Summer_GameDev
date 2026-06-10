#pragma once
#include <DxLib.h>

class UiBase
{
public:
    UiBase();
    virtual ~UiBase();

    virtual void Update() = 0;
    virtual void Draw() = 0;

    int drawOrder_;

protected:

    struct DrawBillboardUi
    {
        int drawUi_;
		VECTOR pos;
        VECTOR scl;
        VECTOR rot;

	};

};
