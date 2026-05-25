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
	for (auto* col : myColliders_)
	{
		if (col != nullptr)
		{
			// ※ もしシステム側からコライダーを登録解除する関数（RemoveCollider 等）があれば、
			// 削除する前にここで呼び出してください。
			// 例: CollisionManager::GetInstance().RemoveCollider(col);

			delete col;
		}
	}
	myColliders_.clear();
}

void IronBall::Update(void)
{
	for (size_t i = 0; i < instances_.size(); ++i) {
		auto& instance = instances_[i];

		// 振り子運動の更新
		Pendulum(instance);
		instance.transform.Update();

		// ★ 重要：DxLibの仕様上、動く3Dモデルの衝突情報を毎フレーム最新にする
		//MV1SetupCollInfo(instance.transform.modelId);
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

	int debugColor = GetColor(0, 255, 0);

	for (auto* col : myColliders_)
	{
		if (col != nullptr)
		{
			// 各コライダーの持つデバッグ用描画関数を呼び出す
			// 内部で DrawSphere3D が実行され、画面にコライダーが表示されます
			col->DrawDebug(debugColor);
		}
	}
}

void IronBall::Release(void)
{
	MV1DeleteModel(transform_.modelId);
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
	// 登録用の連番インデックス
	int index = 0;

	// 複製されたすべてのインスタンスに対して、球体コライダーを個別に生成
	for (auto& instance : instances_)
	{
		// 引数: タグ、追従する個別のTransform、ローカルオフセット座標、半径
		ColliderSphere* colSphere = new ColliderSphere(
			ColliderBase::TAG::IRON_BALL,
			&instance.transform,
			SPHERE_LOCAL_POS,
			SPHERE_RADIUS
		);

		// 自作の管理用リストに保存（UpdateやDrawでの個別処理用）
		myColliders_.push_back(colSphere);

		// ★【重要】基底クラスの ownColliders_ に連番をキーにしてすべて叩き込む！
		// これにより、外部のシステム（StageやSceneなど）が ownColliders_ の中身を
		// 全ループで回収している場合、2個目以降のコライダーも自動的に検知・登録されます。
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
