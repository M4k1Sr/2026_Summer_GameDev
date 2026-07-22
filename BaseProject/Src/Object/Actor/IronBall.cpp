#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Object/Common/Transform.h"
#include "../../Object/Collider/ColliderModel.h"
#include "../../Object/Collider/ColliderSphere.h"
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
	//生成したすべてのコライダーを安全に破棄
	for (auto* col : myColliders_)
	{
		if (col != nullptr)
		{
			delete col;
		}
	}
	myColliders_.clear();
}

void IronBall::Update(void)
{
	//すべての鉄球の挙動を更新
	for (size_t i = 0; i < instances_.size(); ++i) 
	{
		auto& instance = instances_[i];

		Pendulum(instance);		// 振り子運動の更新
		instance.transform.Update();
	}
}

void IronBall::Draw(void)
{
	//モデルの明るさ設定
	float bright = 1.5f;
	COLOR_F color = GetColorF(bright, bright, bright, 1.0f);

	// 全ての鉄球モデルを描画
	for (auto& instance : instances_)
	{
		MV1SetDifColorScale(instance.transform.modelId, color);
		MV1DrawModel(instance.transform.modelId);
	}

	//デバッグ用コライダーの表示戦を描画(緑色)
	//int debugColor = GetColor(0, 255, 0);
	//for (auto* col : myColliders_)
	//{
	//	if (col != nullptr)
	//	{
	//		col->DrawDebug(debugColor);
	//	}
	//}
}

void IronBall::Release(void)
{
	// 生成したすべての鉄球モデルをループで確実に削除する
	for (auto& instance : instances_)
	{
		MV1DeleteModel(instance.transform.modelId);
	}
	instances_.clear();
}

void IronBall::InitLoad(void)
{
	// 指定個数分の鉄球インスタンスを生成・複製
	for (int i = 0; i < INSTANCE_COUNT; ++i) {
		InstanceData data;

		// リソースマネージャーからモデルを複製
		data.transform.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::IRON_BALL));

		// X軸方向に間隔をずらして初期座標を設定
		data.basePos = VAdd(IRON_BALL_POS, VGet(-(i * INTERVAL_X), 0.0f, 0.0f));
		data.transform.scl = IRON_BALL_SCALE;

		// 登場のタイミングを0.5秒ずつずらす
		data.timeOffset = i * 0.5f;

		instances_.push_back(data);
	}
}

void IronBall::InitTransform(void)
{
	// 代表のTransformを初期化
	transform_.scl = IRON_BALL_SCALE;
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Identity();
	transform_.pos = IRON_BALL_POS;
	transform_.Update();
}

void IronBall::InitCollider(void)
{
	// 登録用の連番インデックス
	int index = 0;

	// 生成されたすべての鉄球に対して個別にコライダーを適用
	for (auto& instance : instances_)
	{
		ColliderSphere* colSphere = new ColliderSphere(
			ColliderBase::TAG::IRON_BALL,
			&instance.transform,
			SPHERE_LOCAL_POS,
			SPHERE_RADIUS
		);

		myColliders_.push_back(colSphere);

		// 外部システム（Stageなど）が自動回収できるように連番キーで登録
		ownColliders_.emplace(index, colSphere);
		index++;
	}
}

void IronBall::InitAnimation(void)
{
}

void IronBall::InitPost(void)
{
}

void IronBall::Pendulum(InstanceData& data)
{
	// サイン波を利用した振り子運動の計算（角度制限: 45度）
	float time = (GetNowCount() / 1000.0f) + data.timeOffset;
	float maxDegree = 45.0f;
	float currentDegree = sinf(time * 2.0f) * maxDegree;

	// 回転クォータニオンの計算と適用
	data.transform.quaRot = Quaternion::Euler(AsoUtility::Deg2RadF(currentDegree),0.0f, 0.0f);

	// 支点を中心とした位置の再計算
	VECTOR pivot = VAdd(data.basePos, VGet(0.0f, CHAIN_END_POS, 0.0f));
	VECTOR offset = VGet(0.0f, -CHAIN_END_POS, 0.0f);
	data.transform.pos = VAdd(pivot, data.transform.quaRot.PosAxis(offset));
}
