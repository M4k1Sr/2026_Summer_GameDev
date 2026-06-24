#pragma once
#include <DxLib.h>
#include <map>
#include <vector>
class EffectBase;

class EffectManager
{
public:

    //static void CreateInstance();
    //static void Destroy();
    //static EffectManager& GetInstance();

    EffectManager();
    ~EffectManager();

    void Play();

    int Add(EffectBase* effect);
    EffectBase* GetEffect(int id);
    void Remove(int id);

    void Clear();

private:

    std::map<int, EffectBase*> effects_;
    int nextId_ = 0;
};