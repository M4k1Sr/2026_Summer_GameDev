#include <DxLib.h>
#include <chrono>
#include <EffekseerForDXLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Object/Common/Transform.h"
#include "../../Object/Collider/ColliderModel.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include"../../Manager/SoundManager.h"
#include "../../Application.h"
#include "Stage.h"
#include "../../Manager/ServiceLocator.h"

Stage::Stage(void)
	:
	ActorBase()
{
}

Stage::~Stage(void)
{
}

void Stage::Update(void)
{
	transform_.Update();
}

void Stage::Draw(void)
{
	// モデル描画
	float bright = 1.5f;
	MV1SetDifColorScale(transform_.modelId, GetColorF(bright, bright, bright, 1.0f));
	MV1DrawModel(transform_.modelId);

}

void Stage::Release(void)
{
	//// サウンド停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::BGM_STAGE1);
}

void Stage::InitLoad(void)
{
	// モデル読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FIRST_STAGE));

}

void Stage::InitTransform(void)
{
	transform_.scl = MAIN_STAGE_SCALE;
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Identity(); 
	transform_.pos = MAIN_STAGE_POS;
	transform_.Update();

}

void Stage::InitCollider(void)
{
	// DxLib側の衝突情報セットアップ
	MV1SetupCollInfo(transform_.modelId);

	// モデルのコライダ
	ColliderModel * colModel =
		new ColliderModel(ColliderBase::TAG::STAGE, &transform_);

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

void Stage::InitAnimation(void)
{
}

void Stage::InitPost(void)
{
	ServiceLocator::GetSound().PlayEvent(SOUND_ID::BGM_STAGE1, true);

}
