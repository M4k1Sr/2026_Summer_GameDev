#include "EffectManager.h"
#include "../Base/EffectBase.h"
#include <algorithm>

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
	effects_.clear();
}

void EffectManager::Play()
{
    auto itr = std::remove_if(effects_.begin(), effects_.end(),
        [](EffectBase* t) {

            bool flag = t->Update();
            if (flag) delete t;
            return flag;
        }
    );
    effects_.erase(itr, effects_.end());

    UpdateEffekseer3D();
}

void EffectManager::Draw()
{
    DrawEffekseer3D();
}
void EffectManager::Add(EffectBase* effect)
{
	effects_.emplace_back(effect);
}

void EffectManager::Clear()
{
	for (const auto& i : effects_) {
		delete i;
	}
	effects_.clear();
}


