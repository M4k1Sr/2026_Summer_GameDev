#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
	:
	modelId_(modelId),
	playType_(-1),
	playAnim_(),
	isLoop_(true)
{
}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	Add(type, speed, animation);
}

void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;

	Add(type, speed, animation);
}

void AnimationController::Play(int type, bool isLoop)
{

	if (playType_ == type) return;

	// 既存のブレンドが残ってたら古いのを先にDetach
	if (isBlending_) {
		MV1DetachAnim(modelId_, blendAttachNo_);
		isBlending_ = false;
	}

	if (playType_ != -1) {
		blendAnim_ = playAnim_; // ← 古いアニメ情報を丸ごと保存
		blendAttachNo_ = playAnim_.attachNo;
		isBlending_ = true;
		blendRate_ = 0.0f;
	}

	playType_ = type;
	playAnim_ = animations_[type];
	playAnim_.step = 0.0f;

	if (playAnim_.model == -1)
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	else
		playAnim_.attachNo = MV1AttachAnim(modelId_, 0, playAnim_.model);

	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
	isLoop_ = isLoop;

}

void AnimationController::Update(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 新アニメを進める
	playAnim_.step += deltaTime * playAnim_.speed;
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

	if (isBlending_) {
		// 古いアニメも独自に進める（止めてもOK、好み次第）
		blendAnim_.step += deltaTime * blendAnim_.speed;
		MV1SetAttachAnimTime(modelId_, blendAttachNo_, blendAnim_.step);

		// ブレンド率を進める（0.2秒でブレンド完了）
		blendRate_ += deltaTime / 0.2f;
		blendRate_ = min(blendRate_, 1.0f);

		MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, blendRate_);
		MV1SetAttachAnimBlendRate(modelId_, blendAttachNo_, 1.0f - blendRate_);

		if (blendRate_ >= 1.0f) {
			MV1DetachAnim(modelId_, blendAttachNo_);
			blendAttachNo_ = -1;
			isBlending_ = false;
		}
	}
	else {
		MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, 1.0f);
	}

	// ループ処理
	if (isLoop_ && playAnim_.step >= playAnim_.totalTime) {
		playAnim_.step = 0.0f;
	}
}

void AnimationController::Release(void)
{

	// 外部FBXのモデル(アニメーション)解放
	for (const std::pair<int, Animation>& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}
	
	// 可変長配列をクリアする
	animations_.clear();
	
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

const AnimationController::Animation& AnimationController::GetPlayAnim(void) const
{
	return playAnim_;
}

void AnimationController::Add(int type, float speed, Animation& animation)
{
	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 追加
		animations_.emplace(type, animation);
	}
}

void AnimationController::SetPlaySpeed(float speedMultiplier)
{
	// 念のため、初期状態（何も再生していないとき）は弾く
	if (playType_ == -1) return;

	// animations_ マップに登録されているオリジナルの速度をベースに、
	// 外から渡された倍率（speedMultiplier）を掛け合わせる
	playAnim_.speed = animations_[playType_].speed * speedMultiplier;
}
