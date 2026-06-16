#pragma once
#include <vector>
#include <algorithm>
<<<<<<< HEAD:BaseProject/Src/Renderer/DrawableManager.h
#include "../Renderer/UiRenderer/UiBase.h"
#include "../Renderer/UiRenderer/UiBillboardBase.h"
#include"../Renderer/EffectRenderer/EffectBase.h"
=======
#include "../Base/UiBase.h"
#include "../Base/UiBillboardBase.h"
>>>>>>> m4k:BaseProject/Src/Renderer/UiRenderer/Manager/UIManager.h

class UIManager
{
public:
<<<<<<< HEAD:BaseProject/Src/Renderer/DrawableManager.h
    DrawableManager();
    ~DrawableManager();

    void PlayEffect(int effectHandle, VECTOR pos,VECTOR scale);

=======
    UIManager();
    ~UIManager();
>>>>>>> m4k:BaseProject/Src/Renderer/UiRenderer/Manager/UIManager.h
    void AddUiBase(UiBase* ui);
    void AddUiBillboardBase(UiBillboardBase* uiBillboard);
    void Update();
    void Draw();
    void Release();

private:
    std::vector<UiBase*> uiList_;

    std::vector<UiBillboardBase*> uiBillboardList_;

    std::vector< std::unique_ptr<EffectBase> > effects_;
};