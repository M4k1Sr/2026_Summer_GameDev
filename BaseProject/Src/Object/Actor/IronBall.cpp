#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Object/Common/Transform.h"
#include "../../Object/Collider/ColliderModel.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Application.h"
#include "IronBall.h"

IronBall::IronBall(void)
	:
	ActorBase()
{
}

IronBall::~IronBall(void)
{
}

void IronBall::Update(void)
{
	// --- 振り子の角度計算 ---

		// 1. 時間をもとにサイン波を作る (GetNowCountを使用)
		// 1000.0f で割っている数値を変えると「振るスピード」が変わります
	float time = GetNowCount() / 1000.0f;

	// 2. 振れ幅（角度）を決める
	// 例：45度の範囲で揺らしたい場合
	float maxDegree = 45.0f;
	float currentDegree = sinf(time * 2.0f) * maxDegree; // 2.0fは速さの倍率

	// 3. クォータニオンをその角度で「上書き」する（Multで足し続けないのがコツ）
	transform_.quaRot = Quaternion::Euler(AsoUtility::Deg2RadF(currentDegree), 0.0f, 0.0f);

	// --- 座標の計算（前回と同じ） ---

	// 支点（モデルの初期位置の 100 上）
	VECTOR pivot = VAdd(IRON_BALL_POS, VGet(0.0f, CHAIN_END_POS, 0.0f));

	// 支点から見たモデルの相対位置（100 下）
	VECTOR offset = VGet(0.0f, -CHAIN_END_POS, 0.0f);

	// 回転させたオフセットを支点に足す
	transform_.pos = VAdd(pivot, transform_.quaRot.PosAxis(offset));

	//更新
	transform_.Update();
}

void IronBall::Draw(void)
{
	// モデル描画
	MV1DrawModel(transform_.modelId);

}

void IronBall::Release(void)
{
}

void IronBall::InitLoad(void)
{
	// モデル読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::IRON_BALL));
}

void IronBall::InitTransform(void)
{
	// モデルの大きさ、回転、座標の初期化
	transform_.scl = IRON_BALL_SCALE;
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Identity();
	transform_.pos = IRON_BALL_POS;
	transform_.Update();
}

void IronBall::InitCollider(void)
{
}

void IronBall::InitAnimation(void)
{
}

void IronBall::InitPost(void)
{
}
