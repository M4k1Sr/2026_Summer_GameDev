#pragma once
#include <vector>
#include "BossBase.h"
class BossBase;
class ColliderBase;
class Player;
class ObjectManager;

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

	// プレイヤー情報を受けとる
	void SetPlayer(Player* player);

	// オブジェクト
	const std::vector<BossBase*>& GetBosses(void) const { return bosses_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVからオブジェクト情報の読取を行う
	void LoadCsvData(void);

	//ボスの死亡判定
	bool IsBossDead(void);

	// オブジェクト生成
	BossBase* Create(const BossBase::BossData& data);

	// 現在のステージを取得
	void SetCurrentStage(int stageType) { currentStageType_ = stageType; }

private:

	// ボス
	std::vector<BossBase*> bosses_;

	// プレイヤー
	Player* player_;

	// ステージ番号取得
	int currentStageType_ = 1;


};

