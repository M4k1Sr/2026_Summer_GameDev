#pragma once
#include <vector>
#include "BossBase.h"
class BossBase;
class ColliderBase;

class BossManager
{
public:

	// コンストラクタ
	BossManager(void);

	// デストラクタ
	~BossManager(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// オブジェクト
	const std::vector<BossBase*>& GetBosses(void) const { return bosses_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVからオブジェクト情報の読取を行う
	void LoadCsvData(void);

	// オブジェクト生成
	BossBase* Create(const BossBase::BossData& data);

private:

	// オブジェクト
	std::vector<BossBase*> bosses_;

};

