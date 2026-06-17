#include "./EffectManager.h"
#include "../Base/EffectBase.h"
#include <algorithm>

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
    Clear();
}

void EffectManager::Play()
{
    for (auto itr = effects_.begin(); itr != effects_.end(); ) {
        if (itr->second->Update()) {
            delete itr->second;           
            itr = effects_.erase(itr);    
        }
        else {
            ++itr;           
        }
    }
}

int EffectManager::Add(EffectBase* effect)
{
    int id = nextId_++;
    effects_[id] = effect;
    return id;
}

EffectBase* EffectManager::GetEffect(int id)
{
    auto itr = effects_.find(id);
    if (itr == effects_.end()) return nullptr;
    return itr->second;
}

void EffectManager::Remove(int id)
{
    auto itr = effects_.find(id);
    if (itr != effects_.end()) {
        delete itr->second; // ƒƒ‚ƒŠ‰ğ•ú
        effects_.erase(itr); // ƒ}ƒbƒv‚©‚çæ‚èœ‚­
    }
}

void EffectManager::Clear()
{
    for (auto& [id, effect] : effects_) {
        delete effect;
    }
    effects_.clear();
}


