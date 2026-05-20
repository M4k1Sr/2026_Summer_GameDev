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
	for (auto& instance : instances_) {
		if (instance.colModel != nullptr) {
			delete instance.colModel;
			instance.colModel = nullptr;
		}
	}
}

void IronBall::Update(void)
{
	for (size_t i = 0; i < instances_.size(); ++i) {
		auto& instance = instances_[i];

		// 振り子の挙動で座標・回転を計算
		Pendulum(instance);
		instance.transform.Update();

		// 動く3Dモデルの衝突情報を毎フレーム再構築する
		MV1SetupCollInfo(instance.transform.modelId);
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
	for (auto& instance : instances_)
	{
		// DxLib側の衝突情報セットアップ（個別のモデルIDを指定）
		MV1SetupCollInfo(instance.transform.modelId);

		// 各インスタンスのtransformのアドレスを渡してコライダーを生成
		ColliderModel* colModel =
			new ColliderModel(ColliderBase::TAG::IRON_BALL, &instance.transform);

		// 除外フレーム設定
		for (const std::string& name : EXCLUDE_FRAME_NAMES)
		{
			colModel->AddExcludeFrameIds(name); // プロジェクトの仕様に合わせ単数形(AddExcludeFrameId)に修正
		}

		// 対象フレーム設定
		for (const std::string& name : TARGET_FRAME_NAMES)
		{
			colModel->AddTargetFrameIds(name);
		}

		// インスタンスデータにコライダーを記録
		instance.colModel = colModel;
	}

	// 【注意】衝突マネージャーへの登録に関する補足
	// 基底クラスの ownColliders_ (std::map) は重複キー(COLLIDER_TYPE::MODEL)を持てない設計の場合が多いです。
	// もし衝突マネージャーが ownColliders_ を自動参照する仕組みの場合、暫定として最初の1つのみ登録します。
	if (!instances_.empty())
	{
		ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), instances_[0].colModel);
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
