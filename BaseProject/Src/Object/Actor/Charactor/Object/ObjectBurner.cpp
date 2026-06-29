#include "ObjectBurner.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../../../Collider/ColliderBase.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Application.h"

Burner::Burner(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	moveTimer_(0.0f)
{
}

Burner::~Burner(void)
{
}

void Burner::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::BURNER));

}

void Burner::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void Burner::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::BURNER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::BURNER,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void Burner::InitAnimation(void)
{	
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void Burner::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&Burner::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::STOP),
		std::bind(&Burner::ChangeStateStop, this));

	stateChanges_.emplace(static_cast<int>(STATE::START),
		std::bind(&Burner::ChangeStateStart, this));

	// 初期状態設定
	ChangeState(STATE::START);


	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void Burner::UpdateProcess(void)
{
	// 状態別更新
	stateUpdate_();

	UpdateProcessBurner();
}

void Burner::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void Burner::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));

}

void Burner::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&Burner::UpdateNone, this);

}

void Burner::ChangeStateStart(void)
{

	stateUpdate_ = std::bind(&Burner::UpdateStart, this);

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::START), false);

}

void Burner::ChangeStateStop(void)
{

	stateUpdate_ = std::bind(&Burner::UpdateStop, this);

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::STOP), false);

}

void Burner::UpdateNone(void)
{
}

void Burner::UpdateStart(void)
{
	if (animationController_->IsEnd()) {
		ChangeState(STATE::STOP);
	}
}

void Burner::UpdateStop(void)
{
}

void Burner::UpdateProcessBurner(void)
{
}
