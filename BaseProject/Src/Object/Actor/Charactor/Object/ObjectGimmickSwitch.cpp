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
#include "ObjectGimmickSwitch.h"

ObjectGimmickSwitch::ObjectGimmickSwitch(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	prevPos_(AsoUtility::VECTOR_ZERO),
	velocity_(AsoUtility::VECTOR_ZERO)
{
}

ObjectGimmickSwitch::~ObjectGimmickSwitch(void)
{
}

void ObjectGimmickSwitch::Draw(void)
{
	int matNum = MV1GetMaterialNum(transform_.modelId);
	COLOR_F red = GetColorF(1.0f, 0.0f, 0.0f, 1.0f);
	COLOR_F redStrong = GetColorF(3.0f, 0.0f, 0.0f, 1.0f); // Emiだけ強くする
	COLOR_F black = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < matNum; i++)
	{
		MV1SetMaterialDifColor(transform_.modelId, i, red);
		MV1SetMaterialSpcColor(transform_.modelId, i, black);
		MV1SetMaterialEmiColor(transform_.modelId, i, redStrong); // ← ここを強く
		MV1SetMaterialAmbColor(transform_.modelId, i, red);
	}

	ObjectBase::Draw();
#ifdef _DEBUG

	// 画面左上の座標 (0, 0) から、現在のタイルの座標を表示
	// pos_ は ObjectBase のメンバ変数であると想定しています
	DrawFormatString(50, 100, GetColor(0, 0, 0),
		"switch Pos: x=%6.1f, y=%6.1f, z=%6.1f",
		transform_.pos.x, transform_.pos.y, transform_.pos.z);

	DrawFormatString(70, 120, GetColor(0, 0, 0),
		"Tile Velocity: x=%6.1f, y=%6.1f, z=%6.1f",
		velocity_.x, velocity_.y, velocity_.z);

	// コライダーのデバッグ描画（もしメソッドがあれば）
	for (auto& col : ownColliders_) {
		col.second->Draw();
	}

#endif
}

void ObjectGimmickSwitch::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::OBJECT_COVER));

}

void ObjectGimmickSwitch::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE * 2, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectGimmickSwitch::InitCollider(void)
{

	////主に地面との衝突で使用する線分コライダ
	//ColliderLine* colLine = new ColliderLine(
	//	ColliderBase::TAG::BOSS_GIMMICK_SWITCH, &transform_,
	//	COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	//ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	//モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::BOSS_GIMMICK_SWITCH,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void ObjectGimmickSwitch::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectGimmickSwitch::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}

}

void ObjectGimmickSwitch::UpdateProcess(void)
{
}

void ObjectGimmickSwitch::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void ObjectGimmickSwitch::DrawViewRange(void)
{
}
