#pragma once
#include "PostEffectBase.h"

class MonoShader :
    public PostEffectBase
{
public:

    MonoShader(void);
    ~MonoShader(void) = default;
    void Load(void) override;
	void Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes) override;
	void Release(void) override;

};

