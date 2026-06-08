#pragma once
#include "PostEffectBase.h"

class ScanLineShader:
    public PostEffectBase
{
public:

    ScanLineShader(void);
    ~ScanLineShader(void) = default;
    void Load(void) override;
    void Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes) override;
    void Release(void) override;

private:

    float time_;

};

