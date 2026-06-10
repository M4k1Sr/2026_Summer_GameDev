#pragma once
#include<DxLib.h>
#include <vector>
#include <memory>

class EffectBase;

class EffectManager
{
public:

    static void CreateInstance();
    static void Destroy();
    static EffectManager& GetInstance();

    void Init();
    void Update();
    void Draw();

    void Play(
        int effectHandle,
        VECTOR pos);

private:

    EffectManager() = default;
    ~EffectManager() = default;

private:

    static EffectManager* instance_;

    std::vector<
        std::unique_ptr<EffectBase>
    > effects_;
};