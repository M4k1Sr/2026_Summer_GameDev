#pragma once
#include "PostEffectBase.h"

class BlurShader :
    public PostEffectBase
{
public:

    BlurShader(void);
    ~BlurShader(void) = default;
    void Load(void) override;
	void Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes) override;
	void Release(void) override;


};

