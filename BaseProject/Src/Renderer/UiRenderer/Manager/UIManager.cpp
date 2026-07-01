#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    Release();
}

void UIManager::AddUIBase(UIBase* UI)
{
    // UIのソート
    UIList_.push_back(UI);
    std::sort(UIList_.begin(), UIList_.end(),
        [](UIBase* a, UIBase* b)
        {
            return a->drawOrder_ < b->drawOrder_;
        });

}

void UIManager::AddUIBillboardBase(UIBillboardBase* UIBillboard)
{
    // UIビルボードのソート
    UIBillboardList_.push_back(UIBillboard);
    std::sort(UIBillboardList_.begin(), UIBillboardList_.end(),
        [](UIBillboardBase* a, UIBillboardBase* b)
        {
            return a->drawOrder_ < b->drawOrder_;
        });

}

void UIManager::Update()
{
    for (auto* UI : UIList_)
    {
        UI->Update();
    }

    for (auto* UIBillboard : UIBillboardList_)
    {
        UIBillboard->Update();
    }
}

void UIManager::Draw()
{
    for (auto* UI : UIList_)
    {
        UI->Draw();
    }

    for (auto* UIBillboard : UIBillboardList_)
    {
        UIBillboard->Draw();
    }


}

void UIManager::Release()
{
    for (auto* UI : UIList_)
    {
        delete UI;
    }
    UIList_.clear();

    for (auto* UIBillboard : UIBillboardList_)
    {
        delete UIBillboard;
    }
    UIBillboardList_.clear();
}