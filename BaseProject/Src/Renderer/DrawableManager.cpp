#include "DrawableManager.h"

DrawableManager::DrawableManager()
{
}

DrawableManager::~DrawableManager()
{
    Release();
}

void DrawableManager::Add(UiBase* ui)
{
    uiList_.push_back(ui);
    std::sort(uiList_.begin(), uiList_.end(),
        [](UiBase* a, UiBase* b)
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
}

void DrawableManager::Draw()
{
    for (auto* ui : uiList_)
    {
        ui->Draw();
    }

    
}

void DrawableManager::Release()
{
    for (auto* ui : uiList_)
    {
        delete ui;
    }
    uiList_.clear();
}