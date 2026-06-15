#pragma once
#include<DxLib.h>
#include <vector>
class EffectBase;

class EffectManager
{
public:

    static void CreateInstance();
    static void Destroy();
    static EffectManager& GetInstance();

    EffectManager() = default;
    ~EffectManager() = default;

    void Init();
    void Update();
    void Draw();

    void Play(
        int effectHandle,
        VECTOR pos);


private:

    static EffectManager* instance_;

    std::vector<EffectBase> effects_;

};