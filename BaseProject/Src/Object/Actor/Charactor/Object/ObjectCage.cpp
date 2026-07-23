#include "ObjectCage.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../../../Collider/ColliderBase.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Application.h"
#include "../../../../Renderer/ModelShader/ModelMaterial.h"
#include "../../../../Renderer/ModelShader/ModelRenderer.h"

ObjectCage::ObjectCage(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	moveTimer_(0.0f)
{
}

ObjectCage::~ObjectCage(void)
{
}

void ObjectCage::Draw(void)
{
	renderer_->Draw();
	//if (isAlive_ == true)
	//{
	//	renderer_->Draw();
	//}

	//------------------------------------------------------------------------
	// ディゾルブ処理はこれより上に書く
	//ObjectBase::Draw();
}

void ObjectCage::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::BREAK_CAGE));
	
	//モデル描画用
	material_ = std::make_unique<ModelMaterial>(
		"NoTexVS.cso", 0,
		"NoTexPS.cso", 1
	);

	//以下ディゾルブ処理のための定数バッファ設定
	material_->AddConstBufPS({ 0.0f, 0.0f, 0.0f, 0.0f });
	// ノイズテクスチャを登録 (例として適当なスロット1に)
	int noiseTex = LoadGraph("Data/Image/Noise.png"); // あらかじめ用意したノイズ画像
	material_->SetTextureBuf(1, noiseTex);

	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);
}

void ObjectCage::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectCage::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::CAGE, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::CAGE,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void ObjectCage::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);
}

void ObjectCage::InitPost(void)
{

	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;

	// 自分の transform_ のアドレスをコライダーに叩き込む
	for (auto& col : ownColliders_) {
		col.second->SetFollow(&this->transform_);
	}

	
}

void ObjectCage::UpdateProcess(void)
{

}

void ObjectCage::UpdateProcessPost(void)
{
	transform_.Update();

	if (CheckHitKey(KEY_INPUT_K)) 
	{
		isAlive_ = false;
	}

	if (!isAlive_)
	{
		timer_ += 1.0f;
		float timeRatio = timer_ / duration_;

		if (timeRatio >= 1.1f) {
			transform_.Release();
		}

		float smoothRatio = min(timeRatio, 1.0f);

		// ★ material_ が存在している時だけ安全に呼ぶ
		if (material_ != nullptr)
		{
			material_->SetConstBufPS(0, { smoothRatio, 0.0f, 0.0f, 0.0f });
		}
	}
	
	ObjectBase::UpdateProcessPost();
}
