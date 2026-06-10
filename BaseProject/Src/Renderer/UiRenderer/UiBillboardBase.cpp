#include "UiBillboardBase.h"

UiBillboardBase::UiBillboardBase()
    : drawOrder_(0)
{
}

UiBillboardBase::UiBillboardBase(const DrawBillboardUi& uiData)
{
    uiBillboard_ = new DrawBillboardUi(uiData);
    *uiBillboard_ = uiData;
}

UiBillboardBase::~UiBillboardBase()
{
}