#pragma once
#include "PostEffectBase.h"

class MosaicShader :
    public PostEffectBase
{
public:

    MosaicShader(void);
    ~MosaicShader(void) = default;
    void Load(void) override;
    void Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes) override;
    void Release(void) override;


};

