#include "Clock.h"
#include "../../../Manager/ResourceManager.h"
#include"../Base/UIBase.h"

Clock::Clock(void)
	:
	UIBase(Vector2(CLOCK_POS, CLOCK_POS)),
	maxTime_(120.0f),
	time_(120.0f),
	angle_(0.0f),
	oldTime_(GetNowCount()),
	secondHandImg_(-1),
	handPos_(VGet(HAND_POS_X, HAND_POS_Y, 0.0f)),
	clockImg_(-1),
	centerX_(HAND_CENTER_X),
	centerY_(HAND_CENTER_Y),
	isGameOver_(false)	
{
	//時計
	clockImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CLOCK).handleId_;

	// 秒針
	secondHandImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CLOCK_HAND).handleId_;
}

Clock::~Clock(void)
{
}

void Clock::Update(void)
{

	////時計の回転処理
	ClockProcess();

	//クリアタイム計算
	clearTime_ = static_cast<int>(maxTime_ - time_);
}

void Clock::Draw(void)
{
	//時計描画
	DrawRotaGraph(CLOCK_POS, CLOCK_POS, SCALE, 0.0f, clockImg_, TRUE);

	//秒針描画
	DrawRotaGraph2(handPos_.x, handPos_.y, centerX_, centerY_, HAND_SCALE, angle_, secondHandImg_, TRUE);

}

void Clock::ClockProcess(void)
{
	//現在時間
	int now = GetNowCount();

	//デルタタイム計算
	float deltaTime = (now - oldTime_) / 1000.0f; // ミリ秒を秒に変換

	//現在時間を保存
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
	angle_ = ((1.0f - rate) * DX_TWO_PI_F);

	// 時間制限終了時の処理
	if (!isGameOver_)
	{
		if (time_ <= 0.0f)
		{
			isGameOver_ = true;
		}
	}

	// 保存
	prevAngle_ = angle_;

}

bool Clock::GetIsGameOver(void) const
{
	// 時間制限終了を返す
	return isGameOver_;

}

float Clock::GetMaxTime(void) const
{
	return maxTime_;
}

float Clock::GetTime(void) const
{
	return	clearTime_;
}
