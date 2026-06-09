#include "EffectManager.h"
#include "EffectBase.h"

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
    effects_.push_back(
        std::make_unique<EffectBase>(
            effectHandle,
            pos));
}

void EffectManager::Update()
{
    for (auto& effect : effects_)
    {
        effect->Update();
    }

    effects_.erase(
        std::remove_if(
            effects_.begin(),
            effects_.end(),
            [](const std::unique_ptr<EffectBase>& effect)
            {
                return effect->IsEnd();
            }),
        effects_.end());
}