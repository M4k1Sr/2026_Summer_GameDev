#include "EffectBase.h"

EffectBase::EffectBase(
    int effectHandle,
    VECTOR pos,
    float scale,
    bool isLoop)
{
    effectHandle_ = effectHandle;
    isLoop_ = isLoop;

    playHandle_ =
        PlayEffekseer3DEffect(
            effectHandle_);

    SetScalePlayingEffekseer3DEffect(playHandle_, scale, scale, scale);

    SetPosPlayingEffekseer3DEffect(
        playHandle_,
        pos.x,
        pos.y,
        pos.z);

    isEnd_ = false;
}

bool EffectBase::Update()
{
    if (!IsEffekseer3DEffectPlaying(playHandle_))
    {
        if (isLoop_)
        {
            // çƒê∂ÇµíºÇ∑
            playHandle_ = PlayEffekseer3DEffect(effectHandle_);
            isEnd_ = false;
        }
        else
        {
            isEnd_ = true;
        }
    }

    return isEnd_;
}
bool EffectBase::IsEnd() const
{
    return isEnd_;
}

void EffectBase::SetPosition(VECTOR pos)
{
    SetPosPlayingEffekseer3DEffect(playHandle_, pos.x, pos.y, pos.z);
}