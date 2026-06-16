#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    Release();
}

void UIManager::AddUiBase(UiBase* ui)
{
    // UIのソート
    uiList_.push_back(ui);
    std::sort(uiList_.begin(), uiList_.end(),
        [](UiBase* a, UiBase* b)
        {
            return a->drawOrder_ < b->drawOrder_;
        });

}

void UIManager::AddUiBillboardBase(UiBillboardBase* uiBillboard)
{
    // UIビルボードのソート
    uiBillboardList_.push_back(uiBillboard);
    std::sort(uiBillboardList_.begin(), uiBillboardList_.end(),
        [](UiBillboardBase* a, UiBillboardBase* b)
        {
            return a->drawOrder_ < b->drawOrder_;
        });

}

void UIManager::Update()
{
    for (auto* ui : uiList_)
    {
        ui->Update();
    }

    for (auto* uiBillboard : uiBillboardList_)
    {
        uiBillboard->Update();
    }
}

void UIManager::Draw()
{
    for (auto* ui : uiList_)
    {
        ui->Draw();
    }

    for (auto* uiBillboard : uiBillboardList_)
    {
        uiBillboard->Draw();
    }


}

void UIManager::Release()
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