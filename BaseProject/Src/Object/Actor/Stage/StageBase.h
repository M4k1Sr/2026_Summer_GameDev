#pragma once
#include "../ActorBase.h"
#include <string>
class StageManager;

class StageBase : public ActorBase
{
public:

	// 種別
	enum class TYPE
	{
		FIRST,
		SECOND,
		MAX,
	};

	// オブジェクトデータ
	struct StageData
	{
		int id;					// 種別番号
		std::string modelId;	// モデル情報
		StageBase::TYPE type;	// ステージタイプ
	};

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		MODEL = 0,
		MAX,
	};


	// コンストラクタ
	StageBase(void);
	// デストラクタ
	~StageBase(void) override;

	// 更新
	void Update(void) override {};
	// 描画
	void Draw(void) override {};
	// 解放
	void Release(void) override {};

	// ステージマネージャのセッター
	void SetStageManager(StageManager* stgMng) { stgMng_ = stgMng; }

	// ステージチェンジ
	virtual void StageChange(StageData& data) {};

	int GetId() const { return data_.id; }


protected:

	// リソースロード
	void InitLoad(void) override {};

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {};

	// 衝突判定の初期化
	void InitCollider(void) override {};

	// アニメーションの初期化
	void InitAnimation(void) override {};

	// 初期化後の個別処理
	void InitPost(void) override {};

	// データ
	StageData data_;

private:

	// ステージマネージャ
	StageManager* stgMng_ = nullptr;

};

