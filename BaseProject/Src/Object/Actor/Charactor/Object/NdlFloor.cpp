#include "NdlFloor.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../../../Collider/ColliderBase.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Application.h"

NdlFloor::NdlFloor(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	moveTimer_(0.0f)
{
}

NdlFloor::~NdlFloor(void)
{
}

void NdlFloor::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::NEEDLE_FLOOR));

}

void NdlFloor::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void NdlFloor::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::NEEDLE_FLOOR, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::NEEDLE_FLOOR,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void NdlFloor::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::CLOSE), 10.0f, 0);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::START), 10.0f, 1);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::STOP), 10.0f, 2);

}

void NdlFloor::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&NdlFloor::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::STOP),
		std::bind(&NdlFloor::ChangeStateStop, this));

	stateChanges_.emplace(static_cast<int>(STATE::START),
		std::bind(&NdlFloor::ChangeStateStart, this));

	stateChanges_.emplace(static_cast<int>(STATE::CLOSE),
		std::bind(&NdlFloor::ChangeStateClose, this));

	// 初期状態設定
	ChangeState(STATE::START);


	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void NdlFloor::UpdateProcess(void)
{
	// 状態別更新
	stateUpdate_();

	UpdateProcessNeedle();
}

void NdlFloor::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void NdlFloor::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));

}

void NdlFloor::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&NdlFloor::UpdateNone, this);

}

void NdlFloor::ChangeStateStart(void)
{

	stateUpdate_ = std::bind(&NdlFloor::UpdateStart, this);

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::START), false);

}

void NdlFloor::ChangeStateStop(void)
{

	stateUpdate_ = std::bind(&NdlFloor::UpdateStop, this);

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::STOP), false);

}

void NdlFloor::ChangeStateClose(void)
{
	stateUpdate_ = std::bind(&NdlFloor::UpdateClose, this);

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::CLOSE), false);

}

void NdlFloor::UpdateNone(void)
{
}

void NdlFloor::UpdateStart(void)
{
	if (animationController_->IsEnd()) {
		ChangeState(STATE::STOP);
	}
}

void NdlFloor::UpdateStop(void)
{
	moveTimer_++;

	if (moveTimer_ > 500)
	{
		if (animationController_->IsEnd()) {
			if (state_ == STATE::CLOSE) {
				moveTimer_ = 0;
				ChangeState(STATE::START);
			}
			else {
				moveTimer_ = 0;
				ChangeState(STATE::CLOSE);
			}
		}
	}
}

void NdlFloor::UpdateClose(void)
{
	moveTimer_++;

	if (moveTimer_ > 400)
	{
		if (animationController_->IsEnd()) {
			if (moveTimer_ > 100)
				moveTimer_ = 0;
				ChangeState(STATE::START);
		}
	}
}

void NdlFloor::UpdateProcessNeedle(void)
{
}
