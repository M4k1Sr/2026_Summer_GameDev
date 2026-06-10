#include <DxLib.h>
#include <cmath>
#include "SweatUi.h"
#include "../../Manager/ResourceManager.h"

SweatUi::SweatUi(VECTOR pos)
        : UiBillboardBase()
    ,pos_()
{
    drawOrder_ = 10;
    drawUiH_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SWEAT).handleId_;

}

SweatUi::~SweatUi()
{
}

void SweatUi::Update()
{
}

void SweatUi::Draw()
{
    DrawGraph(pos_.x, pos_.y, drawUiH_, true);
}