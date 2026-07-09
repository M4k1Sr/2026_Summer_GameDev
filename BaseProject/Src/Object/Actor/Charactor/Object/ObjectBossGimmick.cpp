#include <DxLib.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource.h"
#include "../../../../Object/Common/Transform.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderCapsule.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../../../../Application.h"
#include"./ObjectBase.h"
#include "ObjectBossGimmick.h"

ObjectBossGimmick::ObjectBossGimmick(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	prevPos_(AsoUtility::VECTOR_ZERO),
	velocity_(AsoUtility::VECTOR_ZERO)
{
}

ObjectBossGimmick::~ObjectBossGimmick(void)
{
}

int ObjectBossGimmick::GetCnt(void) const
{
	return gimmickOnCnt_;
}

void ObjectBossGimmick::SetCnt(int gimmickOnCnt)
{
	gimmickOnCnt_ = gimmickOnCnt;
}

void ObjectBossGimmick::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::OBJECT_SWITCH));

}

void ObjectBossGimmick::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE * 2, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectBossGimmick::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::BOSS_GIMMICK, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::BOSS_GIMMICK,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void ObjectBossGimmick::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectBossGimmick::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void ObjectBossGimmick::UpdateProcess(void)
{
	ActiveGimmick();
	//jumpPow_ = AsoUtility::VECTOR_ZERO;
}

void ObjectBossGimmick::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void ObjectBossGimmick::DrawViewRange(void)
{
}

void ObjectBossGimmick::ActiveGimmick(void)
{
	if (isGimmick_) {
		// ギミックがアクティブ状態になった場合
		// ギミックのモデルの色が変化
		COLOR_F color = { 50.0f, 0.0f, 0.0f, 1.0f }; // 赤色
		MV1SetMaterialDifColor(transform_.modelId, 0, color); // 赤色に変更
	}



}
