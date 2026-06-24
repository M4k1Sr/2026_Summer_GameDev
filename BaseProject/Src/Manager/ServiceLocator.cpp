#include <cassert>
#include "ServiceLocator.h"
#include "SoundManager.h"
#include "../Renderer/EffectRenderer/Manager/EffectManager.h"
#include "../Renderer/UIRenderer/Manager/UIManager.h"

// ê√ìIÉÅÉìÉoÇÃé¿ëÃ
SoundManager* ServiceLocator::sound_ = nullptr;
EffectManager* ServiceLocator::effect_ = nullptr;
UIManager* ServiceLocator::UI_ = nullptr;

void ServiceLocator::Provide(SoundManager* sound) { sound_ = sound; }
void ServiceLocator::Provide(EffectManager* effect) { effect_ = effect; }
void ServiceLocator::Provide(UIManager* UI) { UI_ = UI; }

SoundManager& ServiceLocator::GetSound()
{
    assert(sound_ != nullptr && "SoundManager is not provided!");
    return *sound_;
}

EffectManager& ServiceLocator::GetEffect()
{
    assert(effect_ != nullptr && "EffectManager is not provided!");
    return *effect_;
}

UIManager& ServiceLocator::GetUI()
{
    assert(UI_ != nullptr && "UIManager is not provided!");
    return *UI_;
}
