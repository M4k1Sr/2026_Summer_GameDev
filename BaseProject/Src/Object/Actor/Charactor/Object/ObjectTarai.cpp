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
#include "./ObjectBossGimmick.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../../../../Application.h"
#include "./ObjectBase.h"
#include "./ObjectManager.h"
#include "ObjectTarai.h"

ObjectTarai::ObjectTarai(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	prevPos_(AsoUtility::VECTOR_ZERO),
	velocity_(AsoUtility::VECTOR_ZERO)
{
}

ObjectTarai::~ObjectTarai(void)
{
}

void ObjectTarai::Draw(void)
{

	ObjectBase::Draw();
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 2b92b11575589e68dd52372e61d8ac9978c2a650
#ifdef _DEBUG

	// 画面左上の座標 (0, 0) から、現在のタイルの座標を表示
	// pos_ は ObjectBase のメンバ変数であると想定しています
	//DrawFormatString(400, 100, GetColor(0, 0, 0),
	//	"Tarai Pos: x=%6.1f, y=%6.1f, z=%6.1f",
	//	transform_.pos.x, transform_.pos.y, transform_.pos.z);

	//DrawFormatString(400, 120, GetColor(0, 0, 0),
	//	"Tarai Velocity: x=%6.1f, y=%6.1f, z=%6.1f",
	//	velocity_.x, velocity_.y, velocity_.z);

	// コライダーのデバッグ描画（もしメソッドがあれば）
	for (auto& col : ownColliders_) {
		col.second->Draw();
	}

#endif
<<<<<<< HEAD
=======
//#ifdef _DEBUG
//
//	// 画面左上の座標 (0, 0) から、現在のタイルの座標を表示
//	// pos_ は ObjectBase のメンバ変数であると想定しています
//	DrawFormatString(400, 100, GetColor(0, 0, 0),
//		"Tarai Pos: x=%6.1f, y=%6.1f, z=%6.1f",
//		transform_.pos.x, transform_.pos.y, transform_.pos.z);
//
//	DrawFormatString(400, 120, GetColor(0, 0, 0),
//		"Tarai Velocity: x=%6.1f, y=%6.1f, z=%6.1f",
//		velocity_.x, velocity_.y, velocity_.z);
//
//	// コライダーのデバッグ描画（もしメソッドがあれば）
//	for (auto& col : ownColliders_) {
//		col.second->Draw();
//	}
//
//#endif
>>>>>>> 5058b032279c409eabf00936443c2f5a982e1e47
=======
>>>>>>> 2b92b11575589e68dd52372e61d8ac9978c2a650
}

void ObjectTarai::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::TARAI));

}

void ObjectTarai::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE / 3, SCALE / 3, SCALE/ 3);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectTarai::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::TARAI, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::TARAI,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);

}

void ObjectTarai::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectTarai::InitPost(void)
{

	// 基底クラスの初期化後処理
	startPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&ObjectTarai::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::STOP),
		std::bind(&ObjectTarai::ChangeStateStop, this));

	stateChanges_.emplace(static_cast<int>(STATE::DOWN),
		std::bind(&ObjectTarai::ChangeStateDown, this));

	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&ObjectTarai::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::STOP);

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void ObjectTarai::UpdateProcess(void)
{
	// 状態別更新
	stateUpdate_();

	ActiveTarai();
}

void ObjectTarai::UpdateProcessPost(void)
{
	// 待機状態の時に移動をリセット
	if (state_ == STATE::STOP)
	{
		jumpPow_ = AsoUtility::VECTOR_ZERO;
		movePow_ = AsoUtility::VECTOR_ZERO;
		transform_.pos = INIT_POS;	
	}

	ObjectBase::UpdateProcessPost();
}

void ObjectTarai::DrawViewRange(void)
{
}

void ObjectTarai::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));

}

void ObjectTarai::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&ObjectTarai::UpdateNone, this);

}

void ObjectTarai::ChangeStateStop(void)
{
	transform_.pos = INIT_POS;

	stateUpdate_ = std::bind(&ObjectTarai::UpdateStop, this);

}

void ObjectTarai::ChangeStateDown(void)
{
	stateUpdate_ = std::bind(&ObjectTarai::UpdateDown, this);

}

void ObjectTarai::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&ObjectTarai::UpdateEnd, this);
}

void ObjectTarai::UpdateNone(void)
{
}

void ObjectTarai::UpdateStop(void)
{
	// ギミック作動したらDown状態に行く
	if (isGimmick_) {
		ChangeState(STATE::DOWN);
	}
}

void ObjectTarai::UpdateDown(void)
{
	UpdateProcessTaraiFall();

	if (transform_.pos.y < 100.0f) {
		isGimmick_ = false;

		ChangeState(STATE::STOP);

	}

}

void ObjectTarai::UpdateEnd(void)
{
}

void ObjectTarai::UpdateProcessTaraiFall(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Application::GetInstance().GetGravityPow() * scnMng_.GetDeltaTime();

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	jumpPow_ = VAdd(jumpPow_, gravity);

	// 重力速度の制限
	if (jumpPow_.y < MAX_TARAI_SPEED)
	{
		jumpPow_.y = MAX_TARAI_SPEED;
	}

}

void ObjectTarai::ActiveTarai(void)
{
	if (isGimmick_) {
		// フラグが立ったらタライが降ってくる
		// タライが敵に命中したらタライはだんだん透過して消えていく
		isTarai_ = true;
	}
}
