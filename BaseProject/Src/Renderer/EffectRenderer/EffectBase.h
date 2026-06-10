#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

class EffectBase
{
public:

    EffectBase
    (
        int effectHandle,
        VECTOR pos
    );

    virtual ~EffectBase() = default;

    virtual void Update();

    bool IsEnd() const;

private:

    int effectHandle_;
    int playHandle_;

    bool isEnd_;
};