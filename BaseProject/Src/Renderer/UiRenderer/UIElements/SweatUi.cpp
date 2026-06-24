#include <DxLib.h>
#include <cmath>
#include "SweatUI.h"
#include "../../../Manager/ResourceManager.h"

SweatUI::SweatUI(VECTOR pos)
        : UIBillboardBase(pos)
{
    drawOrder_ = 10;
    drawUIH_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SWEAT).handleId_;

}

SweatUI::~SweatUI()
{
}

void SweatUI::Update()
{
}

void SweatUI::Draw()
{
    //DrawBillboard3D(pos_, 1.0f, 0.5f, 10.0f, 0.0f, drawUIH_, true);
    DrawGraph3D(pos_.x, pos_.y, pos_.z, drawUIH_, true);

	//DrawGraph((int)pos_.x, (int)pos_.y, drawUIH_, true);
}
