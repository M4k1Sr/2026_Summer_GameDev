#include "UiBase.h"

UiBase::UiBase(Vector2 pos)
    : drawOrder_(0)
{
    uiBase_ = new DrawUi();
    uiBase_->pos_ = pos;
}

UiBase::~UiBase()
{
}