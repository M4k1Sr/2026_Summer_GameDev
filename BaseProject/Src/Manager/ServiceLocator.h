#pragma once

class SoundManager;
class EffectManager;
class UIManager;

class ServiceLocator
{
public:

    static void Provide(SoundManager* sound);
    static void Provide(EffectManager* effect);
    static void Provide(UIManager* ui);

    static SoundManager& GetSound();
    static EffectManager& GetEffect();
    static UIManager& GetUi();

private:

    static SoundManager* sound_;
    static EffectManager* effect_;
    static UIManager* ui_;

};

