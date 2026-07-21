#pragma once
#include "StageBase.h"

class Stage2 : public StageBase
{

protected:

    void InitLoad(void) override;

    void InitTransform(void) override;

    void InitCollider(void) override;

    void InitPost(void) override; // BGMçƒê∂óp
};

