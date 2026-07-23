#include "StageBase.h"
#include <DxLib.h>

StageBase::StageBase(void) : ActorBase() {}
StageBase::~StageBase(void) {}

void StageBase::Update(void)
{
    transform_.Update();
}

void StageBase::Draw(void)
{
    // すべてのステージで共通の描画処理（明るさ補正など）
    float bright = 1.5f;
    MV1SetDifColorScale(transform_.modelId, GetColorF(bright, bright, bright, 1.0f));
    MV1DrawModel(transform_.modelId);
}

void StageBase::Release(void)
{
    // モデルの解放などはActorBase側やResourceManager側で面倒を見ているならここは空でもOK
}

// 子クラスのコライダー初期化を楽にするためのヘルパー関数
void StageBase::SetupModelCollider(ColliderBase::TAG tag, const std::vector<std::string>& excludeFrames, const std::vector<std::string>& targetFrames)
{
    MV1SetupCollInfo(transform_.modelId);
    ColliderModel* colModel = new ColliderModel(tag, &transform_);

    for (const auto& name : excludeFrames) colModel->AddExcludeFrameIds(name);
    for (const auto& name : targetFrames)  colModel->AddTargetFrameIds(name);

    ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}