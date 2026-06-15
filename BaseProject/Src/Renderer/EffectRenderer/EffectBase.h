#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

class EffectBase
{
public:

    EffectBase
    (
        int effectHandle,
        VECTOR pos,
		VECTOR scale
    );

    virtual ~EffectBase() = default;

    virtual void Update();

    bool IsEnd() const;

private:

    int effectHandle_;
    int playHandle_;

    bool isEnd_;
};