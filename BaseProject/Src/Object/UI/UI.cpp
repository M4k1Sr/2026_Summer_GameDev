#include "UI.h"
#include "../../Manager/ResourceManager.h"

UI::UI(void)
	:
	maxTime_(30.0f),
	time_(30.0f),
	angle_(0.0f),
	oldTime_(0),
	secondHandImg_(-1),
	handPos_(VGet(HAND_POS_X, HAND_POS_Y, 0.0f)),
	clockImg_(-1),
	centerX_(HAND_POS_X),
	centerY_(HAND_POS_Y)
{
}

UI::~UI(void)
{
}

void UI::Update(void)
{
	Clock();
}

void UI::Draw(void)
{
	DrawRotaGraph(CLOCK_POS, CLOCK_POS, SCALE, 0.0f, clockImg_, TRUE);

	//DrawRotaGraph2(handPos_.x, handPos_.y, centerX_,centerY_,HAND_SCALE, angle_, secondHandImg_, TRUE);

	DrawRotaGraph(handPos_.x, handPos_.y, HAND_SCALE, angle_, secondHandImg_, TRUE);

}

void UI::Release(void)
{
}

void UI::InitLoad(void)
{
	//時計
	clockImg_ = resMng_.Load(ResourceManager::SRC::OBJECT_CLOCK).handleId_;
	
	// 秒針
	secondHandImg_ = resMng_.Load(ResourceManager::SRC::CLOCK_HAND).handleId_;

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
     //システム起動からの経過時間を取得
	//現在時刻を保存
	//デルタタイム計算に使用
	oldTime_ = GetNowCount();

	//制限時間を初期時間に設定
	time_ = maxTime_;

}

void UI::Clock(void)
{
	//現在時間
	int now = GetNowCount();

	//デルタタイム計算
	float deltaTime = (now - oldTime_) / 1000.0f; // ミリ秒を秒に変換

	oldTime_ = now;
    
	//時間減少
	time_ -= deltaTime;

	//0以下
	if (time_ < 0.0f)
	{
		time_ = 0.0f;
	}

	//残り割合
	float rate = time_ / maxTime_;

	//角度計算
	angle_ = ((1.0f - rate) * DX_TWO_PI_F) - DX_PI_F / 2;
}
