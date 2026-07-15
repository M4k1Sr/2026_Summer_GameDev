#pragma once
#include <vector>
#include "StageBase.h"
class Player;

class StageManager
{
public:

	// コンストラクタ
	StageManager(void);

	// デストラクタ
	~StageManager(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// ステージ
	const std::vector<StageBase*>& GetStage(void) const { return stages_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVからステージ情報の読取を行う
	void LoadCsvData(void);

	// オブジェクト生成
	StageBase* Create(const StageBase::StageData& data);

	// ステージチェンジ
	void StageChange(int nextStageId);



private:

	// オブジェクト
	std::vector<StageBase*> stages_;

	std::map<int, StageBase::StageData> stageMasterMap_;

	// プレイヤー
	Player* player_;

};
