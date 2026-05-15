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
	// 全てのインスタンスを更新
	for (auto& instance : instances_) {
		Pendulum(instance);
		instance.transform.Update();
	}
}

void IronBall::Draw(void)
{
	float bright = 1.5f;
	COLOR_F color = GetColorF(bright, bright, bright, 1.0f);

	// 全てのインスタンスを描画
	for (auto& instance : instances_) {
		MV1SetDifColorScale(instance.transform.modelId, color);
		MV1DrawModel(instance.transform.modelId);
	}
}

void IronBall::Release(void)
{
}

void IronBall::InitLoad(void)
{
	// 指定した個数分、モデルを読み込んで初期化
	for (int i = 0; i < INSTANCE_COUNT; ++i) {
		InstanceData data;

		// モデルの複製（メモリ節約のためDuplicateを使用）
		data.transform.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::IRON_BALL));

		// 座標をずらして設定
		data.basePos = VAdd(IRON_BALL_POS, VGet(0.0f, 0.0f, -(i * INTERVAL_X)));
		data.transform.scl = IRON_BALL_SCALE;

		// タイミングを少しずつずらす（0.5秒ずつ）
		data.timeOffset = i * 0.5f;

		instances_.push_back(data);
	}
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

void IronBall::Pendulum(InstanceData& data)
{
	// 時間計算（個別のオフセットを加算）
	float time = (GetNowCount() / 1000.0f) + data.timeOffset;

	float maxDegree = 45.0f;
	float currentDegree = sinf(time * 2.0f) * maxDegree;

	// 回転の設定
	data.transform.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(currentDegree));

	// 座標計算
	VECTOR pivot = VAdd(data.basePos, VGet(0.0f, CHAIN_END_POS, 0.0f));
	VECTOR offset = VGet(0.0f, -CHAIN_END_POS, 0.0f);
	data.transform.pos = VAdd(pivot, data.transform.quaRot.PosAxis(offset));
}
