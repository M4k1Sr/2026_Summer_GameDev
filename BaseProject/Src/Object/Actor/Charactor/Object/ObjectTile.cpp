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
#include "ObjectTile.h"

ObjectTile::ObjectTile(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	prevPos_(AsoUtility::VECTOR_ZERO),
	velocity_(AsoUtility::VECTOR_ZERO),
	myMoveDir_(AsoUtility::VECTOR_ZERO)
{
	transform_.pos = data.defaultPos;

	// ★CSVの数値(moveType)によって、最初に動き出す方向を4パターンに分岐
	if (data.moveType == 0)
	{
		// パターン0：右から左へ（最初は左へ進む）
		myMoveDir_ = AsoUtility::DIR_F;
	}
	else if (data.moveType == 1)
	{
		// パターン1：左から右へ（最初は右へ進む）
		myMoveDir_ = AsoUtility::DIR_B;
	}
	else if (data.moveType == 2)
	{
		// パターン2：手前から奥へ
		myMoveDir_ = AsoUtility::DIR_R;
	}
	else if (data.moveType == 3)
	{
		// パターン3：奥から手前へ
		myMoveDir_ = AsoUtility::DIR_L;
	}
}

ObjectTile::~ObjectTile(void)
{
}

void ObjectTile::Draw(void)
{
	ObjectBase::Draw();

#ifdef _DEBUG

	////モデルの中央に座標の表示
	//// タイルの少し上（Y座標を少しプラスした位置）の3D空間の座標を設定
	//VECTOR textWorldPos = transform_.pos;
	//textWorldPos.y += 30.0f; // タイルのサイズに合わせて、文字を浮かせる高さを調整してください

	//// 3D空間の座標を、画面上の2D座標（X, Y）に変換する
	//VECTOR screenPos = ConvWorldPosToScreenPos(textWorldPos);

	//// 画面内に収まっている場合のみ描画（カメラの後ろにある時は描画しない）
	//if (screenPos.z >= 0.0f && screenPos.z <= 1.0f)
	//{
	//	// それぞれのタイルの頭上に座標を表示（文字サイズが大きすぎる場合は DrawString を検討）
	//	DrawFormatString(
	//		static_cast<int>(screenPos.x),
	//		static_cast<int>(screenPos.y),
	//		GetColor(255, 255, 255),
	//		"x:%6.1f\ny:%6.1f\nz:%6.1f", // 改行を入れると縦に並んで見やすくなります
	//		transform_.pos.x, transform_.pos.y, transform_.pos.z
	//	);
	//}

	//// 画面左上の座標 (0, 0) から、現在のタイルの座標を表示
	//// pos_ は ObjectBase のメンバ変数であると想定しています
	//DrawFormatString(50, 100, GetColor(0, 0, 0),
	//	"Tile Pos: x=%6.1f, y=%6.1f, z=%6.1f",
	//	transform_.pos.x, transform_.pos.y, transform_.pos.z);

	//DrawFormatString(70, 120, GetColor(0, 0, 0),
	//	"Tile Velocity: x=%6.1f, y=%6.1f, z=%6.1f",
	//	velocity_.x, velocity_.y, velocity_.z);

	// コライダーのデバッグ描画（もしメソッドがあれば）
	for (auto& col : ownColliders_) {
		col.second->Draw();
	}

#endif
}

void ObjectTile::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::OBJECT_TILE));

}

void ObjectTile::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE * 3, SCALE * 100, SCALE * 1.5);
	transform_.scl = VGet(SCALE, SCALE * 2, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectTile::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::TILE, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::TILE,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void ObjectTile::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectTile::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;


	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&ObjectTile::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::STOP),
		std::bind(&ObjectTile::ChangeStateStop, this));

	stateChanges_.emplace(static_cast<int>(STATE::UP),
		std::bind(&ObjectTile::ChangeStateOutward, this));

	stateChanges_.emplace(static_cast<int>(STATE::DOWN),
		std::bind(&ObjectTile::ChangeStateReturn, this));

	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&ObjectTile::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::UP);

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}
}

void ObjectTile::UpdateProcess(void)
{

	// 状態別更新
	stateUpdate_();

}

void ObjectTile::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void ObjectTile::DrawViewRange(void)
{
}

void ObjectTile::ChangeState(STATE state)
{

	state_ = state;

	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));

}

void ObjectTile::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&ObjectTile::UpdateNone, this);

}

void ObjectTile::ChangeStateStop(void)
{
	stateUpdate_ = std::bind(&ObjectTile::UpdateStop, this);

}

void ObjectTile::ChangeStateOutward(void)
{

	//// 経過時間
	//moveTimer_ = 0.0f;
	//// 移動時間
	//moveTime_ = MOVE_TIME;

	//// 初期位置
	//startPos_ = transform_.pos;

	//// 移動する場所
	//movePlacePos_ = VAdd(startPos_, VScale(AsoUtility::DIR_F, MOVE_UP_TILE));

	//stateUpdate_ = std::bind(&ObjectTile::UpdateRight, this);

	// 経過時間
	moveTimer_ = 0.0f;
	moveTime_ = MOVE_TIME;

	// 初期位置
	startPos_ = transform_.pos;

	// 固定の方向ではなく、myMoveDir_ を使って移動先を計算
	movePlacePos_ = VAdd(startPos_, VScale(myMoveDir_, MOVE_UP_TILE));

	stateUpdate_ = std::bind(&ObjectTile::UpdateOutward, this);

}

void ObjectTile::ChangeStateReturn(void)
{
	//// 経過時間
	//moveTimer_ = 0.0f;
	//// 移動時間
	//moveTime_ = MOVE_TIME;

	//// 初期位置
	//startPos_ = transform_.pos;
	//// 移動する場所
	//movePlacePos_ = VAdd(startPos_, VScale(AsoUtility::DIR_B, MOVE_UP_TILE));

	//stateUpdate_ = std::bind(&ObjectTile::UpdateLeft, this);

	// 経過時間
	moveTimer_ = 0.0f;
	moveTime_ = MOVE_TIME;

	// 初期位置
	startPos_ = transform_.pos;

	// myMoveDir_ を反転（-1.0f倍）させて、逆方向（戻る方向）の移動先を計算
	VECTOR reverseDir = VScale(myMoveDir_, -1.0f);
	movePlacePos_ = VAdd(startPos_, VScale(reverseDir, MOVE_UP_TILE));

	stateUpdate_ = std::bind(&ObjectTile::UpdateReturn, this);
}

void ObjectTile::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&ObjectTile::UpdateEnd, this);
}

void ObjectTile::UpdateNone(void)
{
}

void ObjectTile::UpdateStop(void)
{
}

void ObjectTile::UpdateOutward(void)
{

	UpdateProcessFloorMove();
	if (moveTimer_ >= moveTime_)
	{
		ChangeState(STATE::DOWN);
	}

}

void ObjectTile::UpdateReturn(void)
{
	UpdateProcessFloorMove();
	if (moveTimer_ >= moveTime_)
	{
		ChangeState(STATE::UP);
	}

}

void ObjectTile::UpdateEnd(void)
{
}

void ObjectTile::UpdateProcessFloorMove(void)
{
	// 移動前の位置を保存
	prevPos_ = transform_.pos;

	// 経過時間取得(デルタタイム)
	moveTimer_ += SceneManager::GetInstance().GetDeltaTime();
	// 線形補間用ステップ計算
	float t = moveTimer_ / moveTime_;

	// 経過時間により移動完了
	if (moveTimer_ > moveTime_)
	{
		transform_.pos = movePlacePos_;
	}
	else
	{
		// 線形補間で移動(これでオーバーせずにピタッと止まる)
		transform_.pos = AsoUtility::Lerp(startPos_, movePlacePos_, t);
	}

	transform_.Update();

	// モデルを最新行列に合わせる
	MV1RefreshCollInfo(transform_.modelId, -1);

	// 移動速度計算
	velocity_ = VSub(transform_.pos, prevPos_);
}
