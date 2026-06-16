#include "EffectManager.h"
#include "../Base/EffectBase.h"
#include <algorithm>

EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new EffectManager();
    }
}

void EffectManager::Destroy()
{
    delete instance_;
    instance_ = nullptr;
}

EffectManager& EffectManager::GetInstance()
{
    return *instance_;
}

void EffectManager::Init()
{
    effects_.clear();
}

void EffectManager::Play(
    int effectHandle,
    VECTOR pos)
{
    effects_.emplace_back(effectHandle, pos);
}

void EffectManager::Update()
{
    for (auto& effect : effects_)
    {
        effect.Update();
    }

    effects_.erase(
        std::remove_if(
            effects_.begin(),
            effects_.end(),
            [](const EffectBase& effect)
            {
                return effect.IsEnd();
            }),
        effects_.end());

    UpdateEffekseer3D();
}

void EffectManager::Draw()
{
    // EffekseerÇÃï`âÊÇé¿çs
    DrawEffekseer3D();
}