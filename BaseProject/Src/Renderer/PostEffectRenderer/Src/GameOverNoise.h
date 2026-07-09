#pragma once
#include "PostEffectBase.h"

class GameOverNoise :
    public PostEffectBase
{
public:

    GameOverNoise(void);
    ~GameOverNoise(void) = default;
    void Load(void) override;
    void Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes) override;
    void Release(void) override;

private:

    float time_;

};

