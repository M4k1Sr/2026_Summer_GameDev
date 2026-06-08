#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

class EffectBase
{
public:
    EffectBase();
    virtual ~EffectBase();

    bool Load(const char* fileName);

    void Play(VECTOR pos);
    void Stop();

    virtual void Update();
    virtual void Draw();

    bool IsPlaying() const;

protected:

    int effectHandle_;
    int playHandle_;

    VECTOR pos_;
};