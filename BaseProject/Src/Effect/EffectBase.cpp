#include "EffectBase.h"

EffectBase::EffectBase(
    int effectHandle,
    VECTOR pos)
{
    effectHandle_ = effectHandle;

    playHandle_ =
        PlayEffekseer3DEffect(
            effectHandle_);

    SetPosPlayingEffekseer3DEffect(
        playHandle_,
        pos.x,
        pos.y,
        pos.z);


    isEnd_ = false;
}

void EffectBase::Update()
{
    if (isEnd_)
    {
        return;
    }

    if (!IsEffekseer3DEffectPlaying(playHandle_))
    {
        isEnd_ = true;
    }
}

bool EffectBase::IsEnd() const
{
    return isEnd_;
}