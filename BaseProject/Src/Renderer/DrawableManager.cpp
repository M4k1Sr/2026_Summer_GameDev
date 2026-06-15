#include "DrawableManager.h"

DrawableManager::DrawableManager()
{
}

DrawableManager::~DrawableManager()
{
    Release();
}

void DrawableManager::PlayEffect(int effectHandle, VECTOR pos, VECTOR scale)
{
    effects_.push_back(std::make_unique<EffectBase>(effectHandle, pos, scale));
}

void DrawableManager::AddUiBase(UiBase* ui)
{
    // UIのソート
    uiList_.push_back(ui);
    std::sort(uiList_.begin(), uiList_.end(),
        [](UiBase* a, UiBase* b)
        {
            return a->drawOrder_ < b->drawOrder_;
        });
}

void DrawableManager::AddUiBillboardBase(UiBillboardBase* uiBillboard)
{
    // UIビルボードのソート
    uiBillboardList_.push_back(uiBillboard);
    std::sort(uiBillboardList_.begin(), uiBillboardList_.end(),
        [](UiBillboardBase* a, UiBillboardBase* b)
        {
            return a->drawOrder_ < b->drawOrder_;
        });

}

void DrawableManager::Update()
{
    for (auto* ui : uiList_)
    {
        ui->Update();
    }

    for (auto* uiBillboard : uiBillboardList_)
    {
        uiBillboard->Update();
    }

    //エフェクト
     for (auto& effect : effects_) 
     { 
         effect->Update(); 
     }

     //エラーが起きたエフェクトを削除
     effects_.erase(std::remove_if(effects_.begin(), effects_.end(), [](const std::unique_ptr<EffectBase>& effect)
         {
             return effect->IsEnd(); 
         })
         , effects_.end());
     //エフェクトを更新
     UpdateEffekseer3D();
    }

void DrawableManager::Draw()
{

    //エフェクト描画
    DrawEffekseer3D();

    for (auto* ui : uiList_)
    {
        ui->Draw();
    }

    for (auto* uiBillboard : uiBillboardList_)
    {
        uiBillboard->Draw();
    }

    
}

void DrawableManager::Release()
{
    for (auto* ui : uiList_)
    {
        delete ui;
    }
    uiList_.clear();

    for (auto* uiBillboard : uiBillboardList_)
    {
        delete uiBillboard;
    }
    uiBillboardList_.clear();
}