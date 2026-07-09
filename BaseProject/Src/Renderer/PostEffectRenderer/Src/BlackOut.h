#pragma once
#include "PostEffectBase.h"

class BlackOut : public PostEffectBase
{
public:
    BlackOut(void);
    void Load(void) override;
    void Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes) override;
    void Release(void) override;

    // ‰‰o‚ğŠJn‚·‚éŠÖ”
    void StartPucchun() { isPlaying_ = true; timer_ = 0.0f; }

private:
    bool isPlaying_;
    float timer_;
};