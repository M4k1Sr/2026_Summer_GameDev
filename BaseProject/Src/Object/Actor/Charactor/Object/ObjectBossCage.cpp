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
#include "ObjectBossCage.h"

ObjectBossCage::ObjectBossCage(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	prevPos_(AsoUtility::VECTOR_ZERO),
	velocity_(AsoUtility::VECTOR_ZERO)
{
}

ObjectBossCage::~ObjectBossCage(void)
{
}

void ObjectBossCage::Draw(void)
{

	ObjectBase::Draw();
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
}

bool ObjectBossCage::IsCameraFocusing(void) const
{
	return isGimmick_ || (bossFrontDelayTimer_ > 0);
}

void ObjectBossCage::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB));

}

void ObjectBossCage::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE / 3, SCALE / 3, SCALE / 3);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectBossCage::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::OBJECT_BOSS_CAGE, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::OBJECT_BOSS_CAGE,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);

}

void ObjectBossCage::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectBossCage::InitPost(void)
{

	// 基底クラスの初期化後処理
	startPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&ObjectBossCage::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::STOP),
		std::bind(&ObjectBossCage::ChangeStateStop, this));

	stateChanges_.emplace(static_cast<int>(STATE::DOWN),
		std::bind(&ObjectBossCage::ChangeStateDown, this));

	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&ObjectBossCage::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::STOP);

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void ObjectBossCage::UpdateProcess(void)
{
	// 状態別更新
	stateUpdate_();

	ActiveTarai();
}

void ObjectBossCage::UpdateProcessPost(void)
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

void ObjectBossCage::DrawViewRange(void)
{
}

void ObjectBossCage::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));

}

void ObjectBossCage::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&ObjectBossCage::UpdateNone, this);

}

void ObjectBossCage::ChangeStateStop(void)
{
	transform_.pos = INIT_POS;

	stateUpdate_ = std::bind(&ObjectBossCage::UpdateStop, this);

}

void ObjectBossCage::ChangeStateDown(void)
{
	stateUpdate_ = std::bind(&ObjectBossCage::UpdateDown, this);

}

void ObjectBossCage::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&ObjectBossCage::UpdateEnd, this);
}

void ObjectBossCage::UpdateNone(void)
{
}

void ObjectBossCage::UpdateStop(void)
{
	// タイマーが動いている場合はカウントを減らす
	if (bossFrontDelayTimer_ > 0)
	{
		bossFrontDelayTimer_--;
	}

	// ギミック作動したらDown状態に行く
	if (isGimmick_) {
		ChangeState(STATE::DOWN);
	}
}

void ObjectBossCage::UpdateDown(void)
{
	UpdateProcessTaraiFall();

	if (transform_.pos.y < 100.0f) {
		isGimmick_ = false;

		// 地面に着いた瞬間に、カメラを維持したいフレーム数をセット
		bossFrontDelayTimer_ = CAMERA_KEEP_FRAME;
	}

}

void ObjectBossCage::UpdateEnd(void)
{
}

void ObjectBossCage::UpdateProcessTaraiFall(void)
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

void ObjectBossCage::ActiveTarai(void)
{
	if (isGimmick_) {
		// フラグが立ったらタライが降ってくる
		// タライが敵に命中したらタライはだんだん透過して消えていく
		isTarai_ = true;
	}
}
