#pragma once

class SoundManager;
class EffectManager;
class UIManager;

class ServiceLocator
{
public:

    static void Provide(SoundManager* sound);
    static void Provide(EffectManager* effect);
    static void Provide(UIManager* UI);

    static SoundManager& GetSound();
    static EffectManager& GetEffect();
    static UIManager& GetUI();

private:

    static SoundManager* sound_;
    static EffectManager* effect_;
    static UIManager* UI_;

};

