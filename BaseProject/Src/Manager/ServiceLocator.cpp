#include <cassert>
#include "ServiceLocator.h"
#include "SoundManager.h"
#include "../Renderer/EffectRenderer/Manager/EffectManager.h"
#include "../Renderer/UiRenderer/Manager/UIManager.h"

// Ã“Iƒƒ“ƒo‚ÌÀ‘Ì
SoundManager* ServiceLocator::sound_ = nullptr;
EffectManager* ServiceLocator::effect_ = nullptr;
UIManager* ServiceLocator::ui_ = nullptr;

void ServiceLocator::Provide(SoundManager* sound) { sound_ = sound; }
void ServiceLocator::Provide(EffectManager* effect) { effect_ = effect; }
void ServiceLocator::Provide(UIManager* ui) { ui_ = ui; }

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

UIManager& ServiceLocator::GetUi()
{
    assert(ui_ != nullptr && "UiManager is not provided!");
    return *ui_;
}
