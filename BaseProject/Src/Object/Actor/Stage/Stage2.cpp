#include <DxLib.h>
#include <chrono>
#include <EffekseerForDXLib.h>
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/Transform.h"
#include "../../../Object/Collider/ColliderModel.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource.h"
#include"../../../Manager/SoundManager.h"
#include "../../../Application.h"
#include "Stage2.h"
#include "../../../Manager/ServiceLocator.h"

Stage2::Stage2(const StageBase::StageData& data)
	:
	StageBase()
{
	data_ = data;
}

Stage2::~Stage2(void)
{
}

void Stage2::Update(void)
{
	transform_.Update();
}

void Stage2::Draw(void)
{
	// モデル描画
	float bright = 1.5f;
	MV1SetDifColorScale(transform_.modelId, GetColorF(bright, bright, bright, 1.0f));
	MV1DrawModel(transform_.modelId);

}

void Stage2::Release(void)
{
	//// サウンド停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::BGM_STAGE1);
	MV1DeleteModel(transform_.modelId);

}

void Stage2::InitLoad(void)
{
	// モデル読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SECOND_STAGE));

}

void Stage2::InitTransform(void)
{
	transform_.scl = NEXT_STAGE_SCALE;
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Identity();
	transform_.pos = NEXT_STAGE_POS;
	transform_.Update();

}

void Stage2::InitCollider(void)
{
	// DxLib側の衝突情報セットアップ
	MV1SetupCollInfo(transform_.modelId);

	// モデルのコライダ
	ColliderModel* colModel =
		new ColliderModel(ColliderBase::TAG::STAGE2, &transform_);

	// 除外フレーム設定
	for (const std::string& name : EXCLUDE_FRAME_NAMES)
	{
		colModel->AddExcludeFrameIds(name);
	}

	// 対象フレーム設定
	for (const std::string& name : TARGET_FRAME_NAMES)
	{
		colModel->AddTargetFrameIds(name);
	}

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void Stage2::InitPost(void)
{
	ServiceLocator::GetSound().PlayEvent(SOUND_ID::BGM_STAGE1, true);

}
