#pragma once
#include<DxLib.h>
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

    void Draw();

    void Add(EffectBase* effect);

    void Clear();

private:

    std::vector<EffectBase*> effects_;

};