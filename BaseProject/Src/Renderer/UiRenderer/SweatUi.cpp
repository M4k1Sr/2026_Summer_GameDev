#include <DxLib.h>
#include <cmath>
#include "SweatUi.h"
#include "../../Manager/ResourceManager.h"

SweatUi::SweatUi(VECTOR pos)
        : UiBillboardBase(pos)
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
    //DrawBillboard3D(pos_, 1.0f, 0.5f, 10.0f, 0.0f, drawUiH_, true);
    DrawGraph3D(pos_.x, pos_.y, pos_.z, drawUiH_, true);

	//DrawGraph((int)pos_.x, (int)pos_.y, drawUiH_, true);
}
