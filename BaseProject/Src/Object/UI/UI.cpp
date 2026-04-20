#include "UI.h"
#include "../../Manager/ResourceManager.h"

UI::UI(void)
	:
	clock_(-1),
	secondHand_(-1),
	handPos_(VGet(0.0f, 0.0f, 0.0f))
{
}

UI::~UI(void)
{
}

void UI::Update(void)
{
}

void UI::Draw(void)
{
	DrawRotaGraph(CLOCK_POS, CLOCK_POS, SCALE, 0.0f, clock_, TRUE);

	DrawRotaGraph(handPos_.x, handPos_.y, HAND_SCALE, 0.0f, secondHand_, TRUE);
}

void UI::Release(void)
{
}

void UI::InitLoad(void)
{

	//éûåv
	clock_ = resMng_.Load(ResourceManager::SRC::OBJECT_CLOCK).handleId_;
	
	// ïbêj
	secondHand_ = resMng_.Load(ResourceManager::SRC::CLOCK_HAND).handleId_;

	//ïbêjèâä˙ç¿ïW
	handPos_ = VGet(HAND_POS, HAND_POS, 0.0f);
		
}

void UI::InitTransform(void)
{
}

void UI::InitCollider(void)
{
}

void UI::InitAnimation(void)
{
}

void UI::InitPost(void)
{
}

void UI::Clock(void)
{
}
