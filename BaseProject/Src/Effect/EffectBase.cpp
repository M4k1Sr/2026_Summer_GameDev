#include "EffectBase.h"

EffectBase::EffectBase()
{
    effectHandle_ = -1;
    playHandle_ = -1;
}

EffectBase::~EffectBase()
{
    if (effectHandle_ != -1)
    {
        DeleteEffekseerEffect(effectHandle_);
    }
}

bool EffectBase::Load(const char* fileName)
{
    effectHandle_ = LoadEffekseerEffect(fileName);

    return effectHandle_ != -1;
}

void EffectBase::Play(VECTOR pos)
{
    pos_ = pos;

   /* playHandle_ = PlayEffekseer3DEffect(
        effectHandle_,
        pos.x,
        pos.y,
        pos.z
    );*/
}

void EffectBase::Stop()
{
    if (playHandle_ != -1)
    {
        StopEffekseer3DEffect(playHandle_);
    }
}

void EffectBase::Update()
{
}

void EffectBase::Draw()
{
}

bool EffectBase::IsPlaying() const
{
    return IsEffekseer3DEffectPlaying(playHandle_);
}