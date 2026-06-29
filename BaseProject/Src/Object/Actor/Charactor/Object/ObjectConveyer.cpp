#include "ObjectConveyer.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../../../Collider/ColliderBase.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Application.h"

ObjectConveyer::ObjectConveyer(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	moveTimer_(0.0f)
{
}

ObjectConveyer::~ObjectConveyer(void)
{
}

void ObjectConveyer::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::CONVEYER));

}

void ObjectConveyer::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectConveyer::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::CONVEYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::CONVEYER,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void ObjectConveyer::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::ROLL1), 10.0f, 0);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::ROLL2), 10.0f, 7);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::ROLL3), 10.0f, 14);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::ROLL4), 10.0f, 21);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::ROLL5), 10.0f, 28);
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::ROLL6), 10.0f, 35);


	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ROLL1), true);
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ROLL2), true);
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ROLL3), true);
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ROLL4), true);
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ROLL5), true);
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ROLL6), true);

}

void ObjectConveyer::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void ObjectConveyer::UpdateProcess(void)
{
	UpdateProcessConveyer();
}

void ObjectConveyer::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void ObjectConveyer::UpdateProcessConveyer(void)
{
}
