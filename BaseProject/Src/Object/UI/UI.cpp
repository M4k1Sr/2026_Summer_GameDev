#include "UI.h"
#include "../../Manager/ResourceManager.h"

UI::UI(void)
	:
	maxTime_(120.0f),
	time_(120.0f	),
	angle_(0.0f),
	oldTime_(0),
	secondHandImg_(-1),
	handPos_(VGet(HAND_POS_X, HAND_POS_Y, 0.0f)),
	clockImg_(-1),
	centerX_(HAND_CENTER_X),
	centerY_(HAND_CENTER_Y),
	isGameOver_(false)
{
}

UI::~UI(void)
{
}

void UI::Update(void)
{
	Clock();

	//クリアタイム計算
		clearTime_ = static_cast<int>(maxTime_ - time_);
}

void UI::Draw(void)
{
	DrawRotaGraph(CLOCK_POS, CLOCK_POS, SCALE, 0.0f, clockImg_, TRUE);

	DrawRotaGraph2(handPos_.x, handPos_.y, centerX_, centerY_, HAND_SCALE, angle_, secondHandImg_, TRUE);

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

	//2分で１周
	speed_ = DX_TWO_PI_F / 120.0f;

	//制限時間を初期時間に設定
	time_ = maxTime_;

	angle_ = 0.0f ;
}

void UI::Clock(void)
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

bool UI::GetIsGameOver(void) const
{

		return isGameOver_;

}

int UI::GetMaxTime(void) const
{
	return maxTime_;
}

int UI::GetTime(void) const
{
	return clearTime_;
}
